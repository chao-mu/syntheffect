#include "ofGraphics.h"

#include "syntheffect/patch/Pipeline.h"
#include "syntheffect/graphics/PingPongBufferMap.h"

namespace syntheffect {
    namespace patch {
        void Pipeline::draw(shared_ptr<graphics::PingPongBufferMap> channels, float t) {
            shared_ptr<graphics::PingPongBuffer> buf_out = channels->get(out_);

            if (out_ != in_) {
                shared_ptr<graphics::PingPongBuffer> buf_in = channels->get(in_);

                buf_out->begin();
                ofClear(255, 255, 255, 0);
                buf_in->drawable()->draw(0, 0);
                buf_out->end();
            }

            for (auto effect : effects_) {
                if (effect->isActive()) {
                    effect->draw(buf_out, t);
                }
            }
        }

        Pipeline::Pipeline(std::string in, std::string out) : effects_() {
            in_ = in;
            out_ = out;
        }

        void Pipeline::addEffect(shared_ptr<effect::Effect> pipeline_effect) {
            effects_.push_back(pipeline_effect);
        }

        std::string Pipeline::getIn() {
            return in_;
        }

        std::string Pipeline::getOut() {
            return out_;
        }
    }
}