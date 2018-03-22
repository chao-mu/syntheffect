#pragma once

#include <vector>

#include "syntheffect/filter/FilterBase.h"

namespace syntheffect {
    namespace filter {
        class FilterChain : public FilterBase {
            public:
                void begin(float width, float height, float t);
                void end();
                void append(FilterBase filter);
            private:
                std::vector<FilterBase> filters_;
        };
    }
}