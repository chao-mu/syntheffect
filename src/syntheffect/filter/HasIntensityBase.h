#pragma once

#include "ofShader.h"

namespace syntheffect {
    namespace filter {
        class HasIntensityBase {
            public:
                void setIntensity(float intensity, float inputMin, float inputMax);
                float getIntensity(float outputMin, float outputMax);
            private:
                float intensity_;
        };
    }
}