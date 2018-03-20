#include "syntheffect/filter/HasIntensityBase.h"

#include "ofMath.h"

#include <assert.h>

#define INTENSITY_MIN 0.0
#define INTENSITY_MAX 1.0

namespace syntheffect {
    namespace filter {
        float HasIntensityBase::getIntensity(float outputMin, float outputMax) {
            return ofMap(intensity_, INTENSITY_MIN, INTENSITY_MAX, outputMin, outputMax);
        }

        void HasIntensityBase::setIntensity(float intensity, float inputMin, float inputMax) {            
            intensity_ = ofMap(intensity, inputMin, inputMax, INTENSITY_MIN, INTENSITY_MAX);
        }
    }
}

#undef INTENSITY_MIN
#undef INTENSITY_MAX