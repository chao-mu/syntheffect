#pragma once

#include <memory>

#include "syntheffect/graphics/Params.h"
#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace graphics {
        class Drawable {
            public:
                virtual void setup() = 0;
                virtual bool update(std::shared_ptr<Params> params, float t);
                virtual void draw(float x, float y, float width, float height) = 0;
                virtual float getWidth() = 0;
                virtual float getHeight() = 0;
                virtual bool isReady();

                void drawScaleCenter(float dest_w, float dest_h);
        };
    }
}
