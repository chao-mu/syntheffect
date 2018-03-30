#pragma once

#include "syntheffect/filter/FilterBase.h"
#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace patch {
        class Patch {
            public:
                Patch();
                bool load(std::string path);
                void draw(graphics::PingPongBuffer& ping_ping);

            private:
                vector<shared_ptr<filter::FilterBase>> filters_;
                template <typename T> void maybeAddFilter(std::string line, std::string name);
        };
    }
}