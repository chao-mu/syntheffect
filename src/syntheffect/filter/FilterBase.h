#pragma once

#include "ofShader.h"
#include "ofTexture.h"

namespace syntheffect {
    namespace filter {
        class FilterBase {
            public:
                FilterBase();
                virtual void end() = 0;
                virtual void begin(float width, float height, float t) = 0;
                void toggle();
                void start();
                void stop();
                bool isActive();

            protected:
                bool active_;
        };
    }
}