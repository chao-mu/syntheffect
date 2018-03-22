#pragma once

#include <vector>

#include "syntheffect/filter/FilterBase.h"

namespace syntheffect {
    namespace filter {
        class FilterChain : public FilterBase {
            public:
                FilterChain();
                void begin(float width, float height, float t) override;
                void end() override;
                void append(shared_ptr<FilterBase> filter);
            private:
                std::vector<shared_ptr<FilterBase>> filters_;
        };
    }
}