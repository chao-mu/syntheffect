#pragma once

#include "ofShader.h"
#include "ofTexture.h"

namespace syntheffect {
    namespace filter {
        class FilterBase {
            public:
                FilterBase(bool active=false);  
                void end();
                void begin(float width, float height, float t);
                void toggle();
                void start();
                void stop();
                bool isActive();

            protected:
                ofShader shader_;
                bool active_;
                virtual void setupUniforms();
                ofShader getShader();
        };
    }
}