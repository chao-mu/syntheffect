#pragma once

#include "ofXml.h"
#include "ofTexture.h"

#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/graphics/Params.h"

namespace syntheffect {
    namespace graphics {
        class Effect {
            public:
                Effect();

                virtual void drawTo(std::shared_ptr<graphics::PingPongBuffer> ping_ping, float t) = 0;
                void toggle();
                void start();
                void stop();
                bool isActive();
                graphics::Params params;

            private:
                bool active_;
        };
    }
}
