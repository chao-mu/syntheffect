#include "syntheffect/manager/PipelineManager.h"

#include <boost/algorithm/string.hpp>

#include "ofGraphics.h"
#include "ofUtils.h"

#include "syntheffect/graphics/PingPongChannel.h"
#include "syntheffect/graphics/Video.h"
#include "syntheffect/graphics/Image.h"

#define CHANNEL_OUT "out"
#define LAST_NAME_SUFFIX "-last"


namespace syntheffect {
    namespace manager {
        PipelineManager::PipelineManager(int width, int height) : channels_(width, height, GL_RGBA) {
            active_asset_group_  = "";
        }

        void PipelineManager::setup() {
            channels_.allocate(CHANNEL_OUT);
            channels_.allocate(getLastName(CHANNEL_OUT));
        }

        void PipelineManager::setPipelines(const std::vector<settings::PipelineSettings>& pipelines) {
            for (const auto& pipeline_settings : pipelines) {
                std::string out = pipeline_settings.out;

                pipelines_[out] = graphics::Pipeline();
                pipeline_ins_[out] = pipeline_settings.in;

                for (const auto& shader_settings : pipeline_settings.shaders) {
                    auto shader = std::make_shared<graphics::Shader>();
                    shader->load(shader_settings.frag, shader_settings.vert);

                    for (const auto& param_settings : shader_settings.params) {
                        shader->params.set(param_settings);
                    }

                    for (const auto& kv : shader_settings.texture_params) {
                        shader->params.setTexture(kv.first, kv.second);
                    }

                    pipelines_[out].addEffect(shader);
                }
            }
        }

        void PipelineManager::setAssets(const std::vector<settings::AssetGroupSettings>& asset_groups) {
            for (const auto& asset_group : asset_groups) {
                if (active_asset_group_ == "") {
                    active_asset_group_ = asset_group.name;
                }

                // Add the trigger
                settings::ParamSettings trigger = asset_group.trigger;
                trigger.name = getTriggerName(asset_group.name);
                global_params_.set(trigger);

                // Add drawables
                for (const auto& asset : asset_group.assets) {
                    std::shared_ptr<graphics::Drawable> drawable;
                    switch (asset.type) {
                        case settings::ImageType:
                            drawable = std::make_shared<graphics::Image>(asset.path);
                            break;
                        case settings::VideoType:
                            drawable = std::make_shared<graphics::Video>(asset.path, 0);
                            break;
                    }

                    std::string name = asset.name;

                    drawable->setup();
                    drawable_names_[drawable] = asset.name;
                    asset_groups_[asset_group.name].push_back(drawable);

                    channels_.get_or_allocate(name);
                    channels_.get_or_allocate(getLastName(name));
                }
            }
        }


        bool PipelineManager::isFinished() {
            // TODO: Find a way to implement this meaningfully
           return false;
        }

        std::map<std::string, std::shared_ptr<graphics::Drawable>> PipelineManager::getDrawables() {
            std::map<std::string, std::shared_ptr<graphics::Drawable>> drawables;

            for (const auto& kv : asset_groups_) {
                const std::string& group_name = kv.first;
                for (const auto& drawable : kv.second) {
                    std::string name = drawable_names_.at(drawable);
                    if (active_asset_group_ == group_name || drawables.count(name) == 0) {
                        drawables[name] = drawable;
                    }
                }
            }

            return drawables;
        }

        bool PipelineManager::isReady() {
            for (const auto name_and_drawable : getDrawables()) {
                if (!name_and_drawable.second->isReady()) {
                    return false;
                }
            }

            return true;
        }

        void PipelineManager::update(float t) {
            // Select active group
            for (const auto& kv : asset_groups_) {
                const std::string& group_name = kv.first;
                if (group_name != active_asset_group_ && global_params_.getBool(getTriggerName(group_name))) {
                    active_asset_group_ = group_name;
                    break;
                }
            }

            // Update drawables
            for (auto& name_and_drawable : getDrawables()) {
                name_and_drawable.second->update(std::make_shared<param::Params>(), t);
            }
        }

        std::shared_ptr<graphics::Drawable> PipelineManager::render() {
            // Determine current asset group

            // Save previous buffers
            for (const auto& buf_name : channels_.getKeys()) {
                // Save previous buffer
                if (boost::algorithm::ends_with(buf_name, LAST_NAME_SUFFIX)) {
                    continue;
                }

                std::string last_buf_name = getLastName(buf_name);
                std::shared_ptr<graphics::PingPongBuffer> chan = channels_.get(last_buf_name);
                chan->begin();
                ofClear(0);
                channels_.get(buf_name)->drawable()->draw(0, 0);
                chan->end();
            }

            // Set texture parameters
            for (const auto& buf_name : channels_.getKeys()) {
                auto chan = channels_.get(buf_name);
                global_params_.setTexture(buf_name, [chan]() { return chan->drawable()->getTexture(); });
            }

            // Transfer global parameters
            for (auto& id_and_pipeline : pipelines_) {
                for (auto& effect : id_and_pipeline.second.getEffects()) {
                    global_params_.copyTo(effect->params);
                }
            }

            // Write drawables to their destinations
            for (const auto& name_and_drawable : getDrawables()) {
                std::string name = name_and_drawable.first;
                std::shared_ptr<graphics::Drawable> drawable= name_and_drawable.second;

                std::shared_ptr<graphics::PingPongBuffer> chan = channels_.get(name);
                chan->begin();
                ofClear(0);
                drawable->drawScaleCenter(chan->getWidth(), chan->getHeight());
                chan->end();
            }

            // Apply effects and produce output
            for (auto& out_and_pipeline : pipelines_) {
                std::string out_name = out_and_pipeline.first;
                graphics::Pipeline& pipeline = out_and_pipeline.second;
                std::shared_ptr<graphics::PingPongBuffer> in = channels_.get(pipeline_ins_.at(out_name));
                std::shared_ptr<graphics::PingPongBuffer> out = channels_.get_or_allocate(out_name);

                pipeline.drawTo(in, out);
            }

            // Write out channel to last-out channel
            std::shared_ptr<graphics::PingPongBuffer> out = channels_.get(CHANNEL_OUT);

            return std::make_shared<graphics::PingPongChannel>(out);
        }

        void PipelineManager::setGlobalParams(param::Params& params) {
            params.copyTo(global_params_);
        }

        int PipelineManager::getHeight() {
            return channels_.getHeight();
        }

        int PipelineManager::getWidth() {
            return channels_.getWidth();
        }

        std::string PipelineManager::getTriggerName(std::string name) {
            return name + "-trigger";
        }

        std::string PipelineManager::getLastName(std::string buf_name) {
            return buf_name + LAST_NAME_SUFFIX;
        }
    }
}

#undef CHANNEL_OUT
#undef CHANNEL_LAST_OUT
