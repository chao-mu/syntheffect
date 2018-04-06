#pragma once

#include "syntheffect/effect/Effect.h"
#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace patch {
        class Patch {
            public:
                Patch();
                bool load(std::string path);
                void draw(graphics::PingPongBuffer& ping_ping);

            private:
                std::vector<shared_ptr<effect::Effect>> effects_;
                std::map<std::string, shared_ptr<effect::Effect>> effects_by_alias_;
                template <typename T> bool maybeAddFilter(std::string line, std::string name);
        };
    }
}