#include "syntheffect/manager/Manager.h"

#include "ofGraphics.h"

#include "syntheffect/graphics/PingPongChannel.h"

#define CHANNEL_OUT "out"
#define CHANNEL_LAST_OUT "last-out"

namespace syntheffect {
    namespace manager {
        Manager::Manager() {
            channels_ = buildChannels();
        }

        void Manager::setProject(const settings::ProjectSettings& project) {
            for (const auto& pipeline_settings : project.pipelines) {
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

        void Manager::setDrawables(std::vector<std::shared_ptr<graphics::Drawable>> drawables) {
            drawables_ = drawables;

            // Rebuild channels
            channels_ = buildChannels();
        }


        bool Manager::isFinished() {
           if (drawables_.size() > 0) {
               return drawables_[0]->isFinished();
           }

           return false;
        }

        void Manager::addDrawable(std::shared_ptr<graphics::Drawable> drawable) {
            drawables_.push_back(drawable);

            // Rebuild channels
            channels_ = buildChannels();
        }

        std::shared_ptr<syntheffect::graphics::PingPongBufferMap> Manager::buildChannels() {
            int width = 4;
            for (const auto& drawable : drawables_) {
                if (width < drawable->getWidth()) {
                    width = drawable->getWidth();
                }
            }

            int height = 4;
            for (const auto& drawable : drawables_) {
                if (height < drawable->getHeight()) {
                    height = drawable->getHeight();
                }
            }

            auto channels = std::make_shared<syntheffect::graphics::PingPongBufferMap>(width, height, GL_RGB);
            for (size_t i = 0; i < drawables_.size(); i++) {
                std::string name = getChannelName(i);
                channels->allocate(name);
                channels->allocate(getLastName(name));
            }

            channels->allocate(CHANNEL_OUT);
            channels->allocate(CHANNEL_LAST_OUT);

            return channels;
        }


        bool Manager::isReady() {
            for (const auto drawable : drawables_) {
                if (!drawable->isReady()) {
                    return false;
                }
            }

            return true;
        }

        void Manager::update(float t) {
            for (auto& drawable : drawables_) {
                drawable->update(std::make_shared<param::Params>(), t);
            }
        }

        std::shared_ptr<graphics::Drawable> Manager::render() {
            // Write channelN to last-channelN
            for (size_t i = 0; i < drawables_.size(); i++) {
                std::shared_ptr<graphics::PingPongBuffer> chan = channels_->get(getChannelName(i));
                chan->begin();
                ofClear(0);
                drawables_[i]->drawScaleCenter(chan->getWidth(), chan->getHeight());
                chan->end();
            }


            // Set texture parameters
            for (const auto& key : channels_->getKeys()) {
                for (auto& id_and_pipeline : pipelines_) {
                    for (auto& effect : id_and_pipeline.second.getEffects()) {
                        std::shared_ptr<graphics::PingPongBuffer> buf = channels_->get(key);
                        effect->params.setTexture(key, [buf]() { return buf->drawable()->getTexture(); });
                    }
                }
            }

            // Apply effects and produce output
            for (auto& out_and_pipeline : pipelines_) {
                std::string out_name = out_and_pipeline.first;
                graphics::Pipeline& pipeline = out_and_pipeline.second;
                std::shared_ptr<graphics::PingPongBuffer> in = channels_->get(pipeline_ins_.at(out_name));
                std::shared_ptr<graphics::PingPongBuffer> out = channels_->get_or_allocate(out_name);

                pipeline.drawTo(in, out);
            }

            // Write out channel to last-out channel
            std::shared_ptr<graphics::PingPongBuffer> out = channels_->get(CHANNEL_OUT);
            out->drawTo(channels_->get(CHANNEL_LAST_OUT));

            return std::make_shared<graphics::PingPongChannel>(out);
        }

        void Manager::setGlobalEffectParams(param::Params& params) {
            for (auto& id_and_pipeline : pipelines_) {
                for (auto& effect : id_and_pipeline.second.getEffects()) {
                    params.copyTo(effect->params);
                }
            }
        }

        int Manager::getHeight() {
            return channels_->getHeight();
        }

        int Manager::getWidth() {
            return channels_->getWidth();
        }

        std::string Manager::getLastName(std::string buf_name) {
            return "last-" + buf_name;
        }

        std::string Manager::getChannelName(int i) {
            return "channel" + std::to_string(i + 1);
        }
    }
}

#undef CHANNEL_OUT
#undef CHANNEL_LAST_OUT
