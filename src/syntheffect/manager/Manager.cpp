#include "syntheffect/manager/Manager.h"

#include "ofGraphics.h"

#include "syntheffect/graphics/PingPongChannel.h"

#define CHANNEL_OUT "out"
#define CHANNEL_LAST_OUT "last-out"

namespace syntheffect {
    namespace manager {
        Manager::Manager() {
            current_id_ = 0;
            channels_ = buildChannels();
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


        int Manager::nextID() {
            return current_id_++;
        }

        int Manager::addPipeline() {
            int id = nextID();
            pipelines_[id] = graphics::Pipeline();

            return id;
        }

        void Manager::connectPipelineOut(int pipeline_id, std::string out) {
            pipeline_outs_[pipeline_id] = out;
        }

        void Manager::connectPipelineIn(int pipeline_id, std::string in) {
            pipeline_ins_[pipeline_id] = in;
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
                        effect->params.set(key, [buf](param::ParamAccessors&) { return buf->drawable()->getTexture(); });
                    }
                }
            }

            // Apply effects and produce output
            for (auto& id_and_pipeline : pipelines_) {
                int id = id_and_pipeline.first;
                graphics::Pipeline& pipeline = id_and_pipeline.second;
                std::shared_ptr<graphics::PingPongBuffer> in = channels_->get(pipeline_ins_.at(id));
                std::shared_ptr<graphics::PingPongBuffer> out = channels_->get_or_allocate(pipeline_outs_.at(id));

                pipeline.drawTo(in, out);
            }

            // Write out channel to last-out channel
            std::shared_ptr<graphics::PingPongBuffer> out = channels_->get(CHANNEL_OUT);
            out->drawTo(channels_->get(CHANNEL_LAST_OUT));

            return std::make_shared<graphics::PingPongChannel>(out);
        }

        int Manager::appendShaderEffect(int pipeline_id, std::string frag, std::string vert) {
            std::shared_ptr<graphics::Shader> effect = std::make_shared<graphics::Shader>();

            effect->load(frag, vert);

            return pipelines_.at(pipeline_id).addEffect(effect);
        }

        void Manager::setEffectParams(int pipeline_id, int effects_index, param::Params& params) {
            params.copyTo(pipelines_.at(pipeline_id).getEffects().at(effects_index)->params);
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
