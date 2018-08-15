#pragma once

#include <memory>

#include "syntheffect/param/Params.h"
#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace graphics {
        class Drawable {
            public:
                virtual void setup();
                virtual void update(float t, param::Params& params);
                virtual void draw(float x, float y, float width, float height) = 0;
                virtual float getWidth() = 0;
                virtual float getHeight() = 0;
                virtual bool isReady();
                virtual bool isFinished();

                void drawScaleCenter(float dest_w, float dest_h);
        };
    }
}
