#include "syntheffect/filter/FilterBase.h"

namespace syntheffect {
    namespace filter{
        FilterBase::FilterBase() {
            active_ = true;
        }

        bool FilterBase::isActive() {
            return active_;
        }

        void FilterBase::toggle() {
            if (isActive()) {
                stop();
            } else {
                start();
            }
        }

        void FilterBase::start() {
            active_ = true;
        }

        void FilterBase::stop() {
            active_ = false;
        }
    }
}