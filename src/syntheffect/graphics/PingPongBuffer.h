#ifndef SYNTHEFFECT_GRAPHICS_PINGPONGBUFFER_H
#define SYNTHEFFECT_GRAPHICS_PINGPONGBUFFER_H

#pragma once

#include <memory>

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

                ofFbo& dest();
                ofFbo& source();

                void swap();

            private:
                std::shared_ptr<ofFbo> src_;
                std::shared_ptr<ofFbo> dest_;
                bool receiving_;
        };
    }
}

#endif
