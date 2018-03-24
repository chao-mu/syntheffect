#pragma once

#include "ofFbo.h"

namespace syntheffect {
    namespace graphics {
        class PingPongBuffer {
            public:
                PingPongBuffer();
                void allocate(int width, int height, int internal_format=GL_RGBA);
                bool isAllocated();
                void swap();
                float getHeight();
                float getWidth();

                shared_ptr<ofFbo> src;
                shared_ptr<ofFbo> dest;

            private:
                vector<shared_ptr<ofFbo>> fbos_;
        };
    }
}