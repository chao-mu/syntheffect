#pragma once

#include "ofTexture.h"

#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/param/Params.h"

namespace syntheffect {
    namespace graphics {
        class Effect {
            public:
                Effect();

                virtual void drawTo(std::shared_ptr<graphics::PingPongBuffer> ping_ping) = 0;
                void toggle();
                void start();
                void stop();
                bool isActive();
                param::Params params;

            private:
                bool active_;
        };
    }
}
