#include "syntheffect/render/Renderer.h"

#include <boost/algorithm/string.hpp>

#include "ofGraphics.h"
#include "ofPixels.h"
#include "ofImage.h"

#include "syntheffect/asset/PingPongChannel.h"

#define CHANNEL_OUT "out"
#define LAST_NAME_SUFFIX "-last"

namespace syntheffect {
    namespace render {
        Renderer::Renderer(int width, int height) : channels_(width, height, GL_RGBA) {
        }

        void Renderer::setup() {
            channels_.allocate(CHANNEL_OUT);
            channels_.allocate(getLastName(CHANNEL_OUT));
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

        void Renderer::update(param::Params& params, const std::vector<std::shared_ptr<asset::Asset>>& assets) {
            // Initialize channels that have not been allocated yet
            for (const auto& a : assets) {
                channels_.get_or_allocate(a->getName());
                channels_.get_or_allocate(getLastName(a->getName()));
            }

            for (const auto& pipeline : pipelines_) {
                channels_.get_or_allocate(pipeline->getOut());
                channels_.get_or_allocate(getLastName(pipeline->getOut()));
            }

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

            // Write drawables to their destinations
            for (auto a : assets) {
                std::shared_ptr<graphics::PingPongBuffer> chan = channels_.get(a->getName());
                chan->begin();
                ofClear(0);
                a->drawScaleCenter(chan->getWidth(), chan->getHeight());
                chan->end();
            }

            // Set texture parameters
            for (const auto& buf_name : channels_.getKeys()) {
                auto chan = channels_.get(buf_name);
                params.setTexture(buf_name, [chan]() { return chan->drawable()->getTexture(); });
            }

            // Transfer parameters
            for (auto pipeline : pipelines_) {
                for (auto& effect : pipeline->getEffects()) {
                    params.copyTo(effect->params);
                }
            }

            // Apply effects and produce output
            for (auto pipeline : pipelines_) {
                std::shared_ptr<graphics::PingPongBuffer> in = channels_.get(pipeline->getIn());
                std::shared_ptr<graphics::PingPongBuffer> out = channels_.get_or_allocate(pipeline->getOut());
                pipeline->drawTo(in, out);
            }
        }

        std::string Renderer::getLastName(std::string buf_name) {
            return buf_name + LAST_NAME_SUFFIX;
        }


        void Renderer::draw(int width, int height) {
            auto out = asset::PingPongChannel(CHANNEL_OUT, channels_.get(CHANNEL_OUT));
            out.drawScaleCenter(width, height);
        }

        void Renderer::saveImage(std::string path) {
            ofPixels pixels;
            channels_.get(CHANNEL_OUT)->drawable()->readToPixels(pixels);

            ofImage image;
            image.setFromPixels(pixels);
            image.save(path);
        }
    }
}

#undef CHANNEL_OUT
#undef LAST_NAME_SUFFIX