#pragma once

#include "ofShader.h"
#include "ofTexture.h"

namespace syntheffect {
    namespace filter {
        class FilterBase {
            public:
                FilterBase(bool active=false);
                virtual void end() {};
                virtual void begin(float width, float height, float t) {};
                void toggle();
                void start();
                void stop();
                bool isActive();

            protected:
                bool active_;
        };
    }
}