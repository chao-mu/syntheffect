#pragma once

#include <memory>

#include "ofFbo.h"

#include "syntheffect/graphics/Drawable.h"

namespace syntheffect {
    namespace graphics {
        class PingPongBuffer {
            public:
                PingPongBuffer();
                void allocate(int width, int height, int internal_format=GL_RGBA);
                bool isAllocated();
                float getHeight();
                float getWidth();

                void begin();
                void end();
                void drawTo(std::shared_ptr<PingPongBuffer> buff, int x=0, int y=0);

                std::shared_ptr<ofFbo> drawable();

            private:
                void swap();

                std::shared_ptr<ofFbo> src_;
                std::shared_ptr<ofFbo> dest_;
                bool receiving_;
        };
    }
}
