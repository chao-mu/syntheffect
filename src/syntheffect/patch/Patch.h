#pragma once

#include "syntheffect/effect/Effect.h"
#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace patch {
        class Patch {
            public:
                Patch();
                void draw(graphics::PingPongBuffer& ping_ping);
                void addEffect(std::string id, shared_ptr<effect::Effect> pipeline_effect);

            private:
                std::vector<shared_ptr<effect::Effect>> effects_;
                std::map<std::string, shared_ptr<effect::Effect>> effects_by_id_;
        };
    }
}