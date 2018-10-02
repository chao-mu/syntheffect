#include "syntheffect/render/Renderer.h"

#include <boost/algorithm/string.hpp>

#include "ofGraphics.h"
#include "ofPixels.h"
#include "ofImage.h"

#include "syntheffect/asset/PingPongChannel.h"
#include "syntheffect/param/Param.h"

#define CHANNEL_OUT "out"
#define LAST_NAME_SUFFIX "-last"

namespace syntheffect {
    namespace render {
        Renderer::Renderer(int width, int height) : width_(width), height_(height), channels_(GL_RGBA) {
        }

        void Renderer::setup() {
            channels_.get_or_allocate(CHANNEL_OUT, width_, height_);
            channels_.get_or_allocate(getLastName(CHANNEL_OUT), width_, height_);
        }

        void Renderer::setPipelines(const std::vector<settings::PipelineSettings>& pipelines) {
            for (const auto& pipeline_settings : pipelines) {
                auto pipeline = std::make_shared<Pipeline>(pipeline_settings.in, pipeline_settings.out);

                pipelines_.push_back(pipeline);

                for (const auto& shader_settings : pipeline_settings.shaders) {
                    auto shader = std::make_shared<graphics::Shader>();
                    shader->load(shader_settings.frag, shader_settings.vert);

                    for (const auto& param_settings : shader_settings.params) {
                        shader->params.set(param_settings);
                    }

                    for (const auto& kv : shader_settings.texture_params) {
                        shader->params.setTexture(kv.first, kv.second);
                    }

                    pipeline->addEffect(shader);
                }
            }
        }

        void Renderer::update(param::Params& params, const std::vector<std::shared_ptr<asset::Drawable>>& assets) {
            // Create handy lookup
            std::map<std::string, std::string> stack_to_asset;
            for (auto const& a : assets) {
                if (!a->getStack().empty() && a->isActive()) {
                    stack_to_asset[a->getStack()] = a->getID();
                }
            }

            // Initialize channels that have not been allocated yet
            for (const auto& a : assets) {
                channels_.get_or_allocate(a->getID(), a->getWidth(), a->getHeight());
                channels_.get_or_allocate(getLastName(a->getID()), a->getWidth(), a->getHeight());
            }

            for (const auto& pipeline : pipelines_) {
                int width = width_;
                int height = height_;

                if (channels_.exists(pipeline->getIn())) {
                    auto source_buf = channels_.at(pipeline->getIn());
                    width = source_buf->getWidth();
                    height = source_buf->getHeight();
                }

                channels_.get_or_allocate(pipeline->getOut(), width, height);
                channels_.get_or_allocate(getLastName(pipeline->getOut()), width, height);
            }

            for (const auto& kv : stack_to_asset) {
                channels_.get_or_allocate(getLastName(kv.first), width_, height_);
            }

            // Save previous buffers
            for (const auto& buf_name : channels_.getKeys()) {
                // Save previous buffer
                if (boost::algorithm::ends_with(buf_name, LAST_NAME_SUFFIX)) {
                    continue;
                }

                std::string last_buf_name = getLastName(buf_name);
                std::shared_ptr<graphics::PingPongBuffer> chan = channels_.at(last_buf_name);
                chan->begin();
                ofClear(0);
                channels_.at(buf_name)->drawable()->draw(0, 0);
                chan->end();
            }

            // Write drawables to their destinations and record which are new frames
            std::map<std::string, bool> new_frames;
            for (auto a : assets) {
                bool is_new = a->isFrameNew();

                std::string id = a->getID();
                params.set(param::Param::boolValue("$" + id + "-new_frame", is_new));
                new_frames[id] = is_new;

                std::shared_ptr<graphics::PingPongBuffer> chan = channels_.at(id);
                chan->begin();
                ofClear(0);
                a->drawScaleCenter(chan->getWidth(), chan->getHeight());
                chan->end();
            }

            // Translate new frame info for stacks
            for (const auto& kv : stack_to_asset) {
                params.set(param::Param::boolValue("$" + kv.first + "-new_frame", new_frames.at(kv.second)));
                new_frames[kv.first] = new_frames[kv.second];
            }

            // Set texture parameters for channels
            for (const auto& buf_name : channels_.getKeys()) {
                auto chan = channels_.at(buf_name);
                params.setTexture(buf_name, [chan]() { return chan->drawable()->getTexture(); });
            }

            // Set texture parameters for stacks
            for (const auto& kv : stack_to_asset) {
                auto chan = channels_.at(kv.second);
                params.setTexture(kv.first, [chan]() { return chan->drawable()->getTexture(); });
            }

            // Transfer parameters
            for (auto pipeline : pipelines_) {
                for (auto& effect : pipeline->getEffects()) {
                    params.copyTo(effect->params);
                }
            }

            // Apply effects and produce output
            for (auto pipeline : pipelines_) {
                std::string in_name = lookupName(stack_to_asset, pipeline->getIn());
                std::string out_name = lookupName(stack_to_asset, pipeline->getOut());

                std::shared_ptr<graphics::PingPongBuffer> in = channels_.at(in_name);
                std::shared_ptr<graphics::PingPongBuffer> out = channels_.at(out_name);

                pipeline->drawTo(in, out);
            }
        }

        std::string Renderer::lookupName(std::map<std::string, std::string> lookup, std::string name) {
            if (lookup.count(name)) {
                return lookup.at(name);
            }

            return name;
        }

        std::string Renderer::getLastName(const std::string& buf_name) {
            return buf_name + LAST_NAME_SUFFIX;
        }

        void Renderer::draw(int width, int height) {
            auto out = asset::PingPongChannel({CHANNEL_OUT}, channels_.at(CHANNEL_OUT));
            out.drawScaleCenter(width, height);
        }

        void Renderer::saveImage(const std::string& path) {
            ofPixels pixels;
            channels_.at(CHANNEL_OUT)->drawable()->readToPixels(pixels);

            ofImage image;
            image.setFromPixels(pixels);
            image.save(path);
        }
    }
}

#undef CHANNEL_OUT
#undef LAST_NAME_SUFFIX
