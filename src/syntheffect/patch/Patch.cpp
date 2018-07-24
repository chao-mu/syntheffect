#include "syntheffect/patch/Patch.h"

#include <string>

#include "ofGraphics.h"

#include "syntheffect/graphics/Shader.h"

namespace syntheffect {
    namespace patch {
        Patch::Patch() : pipelines_() {
        }

        void Patch::addPipeline(std::shared_ptr<Pipeline> pipeline) {
            pipelines_.push_back(pipeline);
        }

        void Patch::setEffectParams(std::shared_ptr<param::Params> params) {
            for (auto pipeline : pipelines_) {
                for (auto effect : pipeline->getEffects()) {
                    effect->params.set(params);
                }
            }
        }

        void Patch::drawTo(std::shared_ptr<graphics::PingPongBufferMap> channels) {
            for (auto pipeline : pipelines_) {
                pipeline->drawTo(channels);
            }
        }
    }
}
