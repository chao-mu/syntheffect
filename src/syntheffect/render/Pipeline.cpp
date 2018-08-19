#include "syntheffect/render/Pipeline.h"

#include "ofGraphics.h"

namespace syntheffect {
    namespace render {
        Pipeline::Pipeline(std::string in, std::string out) {
            in_ = in;
            out_ = out;
        }

        std::string Pipeline::getIn() {
            return in_;
        }

        void Pipeline::setIn(std::string in) {
            in_ = in;
        }

        std::string Pipeline::getOut() {
            return out_;
        }

        void Pipeline::setOut(std::string out) {
            out_ = out;
        }


        void Pipeline::drawTo(std::shared_ptr<graphics::PingPongBuffer> buf_in, std::shared_ptr<graphics::PingPongBuffer> buf_out) {
            // Replace buf out with the contents of buf in so we can apply effects
            buf_out->begin();
            ofClear(0);
            buf_in->drawable()->draw(0, 0);
            buf_out->end();

            // Apply effects to buf out (which starts as buf in)
            for (auto effect : effects_) {
                if (effect->isActive()) {
                    effect->drawTo(buf_out);
                }
            }
        }

        int Pipeline::addEffect(std::shared_ptr<graphics::Effect> pipeline_effect) {
            effects_.push_back(pipeline_effect);

            return effects_.size() - 1;
        }

        std::vector<std::shared_ptr<graphics::Effect>> Pipeline::getEffects() {
            return effects_;
        }
    }
}
