#include "syntheffect/filter/FilterChain.h"

namespace syntheffect {
    namespace filter {
        FilterChain::FilterChain() : FilterBase(), filters_() {
        }

        void FilterChain::append(shared_ptr<FilterBase> filter) {
            filters_.push_back(filter);
        }

        void FilterChain::begin(float width, float height, float t) {
            if (isActive()) {
                for(auto filter: filters_) {
                    filter->begin(width, height, t);
                }
            }
        }

        void FilterChain::end() {
            if (isActive()) {
                for(auto filter: filters_) {
                    filter->end();
                }
            }
        }
    }
}