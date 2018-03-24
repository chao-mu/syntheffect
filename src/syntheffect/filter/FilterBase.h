#pragma once

#include "ofShader.h"
#include "ofTexture.h"
#include "of3dPrimitives.h"

#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace filter {
        class FilterBase {
            public:
                FilterBase();
                virtual void draw(graphics::PingPongBuffer& ping_ping, float t) = 0;
                void toggle();
                void start();
                void stop();
                bool isActive();

            protected:
                bool active_;
        };
    }
}