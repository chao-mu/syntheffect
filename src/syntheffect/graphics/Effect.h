#ifndef SYNTHEFFECT_GRAPHICS_EFFECT_H
#define SYNTHEFFECT_GRAPHICS_EFFECT_H

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
                void start();
                void stop();
                bool isActive();
                param::Params params;

            private:
                bool active_;
        };
    }
}

#endif
