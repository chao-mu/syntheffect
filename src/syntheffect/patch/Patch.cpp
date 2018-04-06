#include "syntheffect/patch/Patch.h"

#include <string>

#include "ofUtils.h"

#include "ofXml.h"

#include "syntheffect/effect/Shader.h"

namespace syntheffect {
    namespace patch {
        Patch::Patch() : effects_(), effects_by_id_() {
        }

        void Patch::addEffect(std::string id, shared_ptr<effect::Effect> pipeline_effect) {
            effects_.push_back(pipeline_effect);
            effects_by_id_[id] = pipeline_effect;
        }

        void Patch::draw(graphics::PingPongBuffer& ping_pong) {
            float t = ofGetElapsedTimef();
            for (auto effect: effects_) {
                if (effect->isActive()) {
                    ping_pong.swap();
                    effect->draw(ping_pong, t);
                }
            }
        }
    }
}