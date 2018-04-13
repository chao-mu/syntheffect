#pragma once

#include "ofFbo.h"

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

                shared_ptr<ofFbo> drawable();

            private:
                void swap();

                shared_ptr<ofFbo> src_;
                shared_ptr<ofFbo> dest_;
                bool receiving_;
        };
    }
}