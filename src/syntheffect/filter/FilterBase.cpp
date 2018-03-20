#include "syntheffect/filter/FilterBase.h"

namespace syntheffect {
    namespace filter{
        FilterBase::FilterBase(bool active) {
            active_ = active;
        }

        void FilterBase::end() {
            if (isActive()) {
                shader_.end();
            }
        }

        void FilterBase::setupUniforms() {
        
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

        void FilterBase::begin(float width, float height, float t) {
            if (isActive()) {
                shader_.begin();
                shader_.setUniform2f("resolution", width, height);
                shader_.setUniform1f("time", t);
                setupUniforms();
            }
        }
    }
}