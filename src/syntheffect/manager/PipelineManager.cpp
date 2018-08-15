#include "syntheffect/manager/PipelineManager.h"

#include "syntheffect/graphics/PingPongChannel.h"

namespace syntheffect {
    namespace manager {
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

        std::shared_ptr<graphics::Drawable> PipelineManager::render(float t, param::Params& params, graphics::PingPongBufferMap& channels) {
            // Transfer global parameters
            for (auto& id_and_pipeline : pipelines_) {
                for (auto& effect : id_and_pipeline.second.getEffects()) {
                    params.copyTo(effect->params);
                }
            }

            // Apply effects and produce output
            for (auto& out_and_pipeline : pipelines_) {
                std::string out_name = out_and_pipeline.first;
                graphics::Pipeline& pipeline = out_and_pipeline.second;
                std::shared_ptr<graphics::PingPongBuffer> in = channels.get(pipeline_ins_.at(out_name));
                std::shared_ptr<graphics::PingPongBuffer> out = channels.get_or_allocate(out_name);

                pipeline.drawTo(in, out);
            }

            // Write out channel to last-out channel
            // TODO: Don't hard code this, create a header of shared upon convensions or something
            // Or make a subclass of PongPongBufferMap that has a getOut()
            std::shared_ptr<graphics::PingPongBuffer> out = channels.get("out");

            return std::make_shared<graphics::PingPongChannel>(out);
        }
    }
}

