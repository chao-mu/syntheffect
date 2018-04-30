#include "syntheffect/patch/Patch.h"

#include <string>

#include "ofGraphics.h"

#include "syntheffect/effect/Shader.h"

namespace syntheffect {
    namespace patch {
        Patch::Patch() : pipelines_() {
        }

        void Patch::addPipeline(shared_ptr<Pipeline> pipeline) {
            pipelines_.push_back(pipeline);
        }

        void Patch::drawTo(shared_ptr<graphics::PingPongBufferMap> channels, float t) {
            for (auto pipeline : pipelines_) {
                pipeline->drawTo(channels, t);
            }
        }
    }
}