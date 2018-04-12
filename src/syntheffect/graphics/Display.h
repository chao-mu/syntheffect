#pragma once

#include "syntheffect/graphics/PingPongBufferMap.h"

namespace syntheffect {
    namespace graphics {
        class Display {
            public:
                Display();
                void allocate(float w, float h, int window_width, int window_height);
                void windowResized(int window_width, int window_height);
                bool isAllocated();
                ofTexture getLastTexture();
                void draw(shared_ptr<graphics::PingPongBuffer> ping_pong);
                void setupDrawSize();

            private:
                ofBufferObject buffer_copy_;
                ofTexture last_texture_;
                float width_;
                float height_;
                float window_width_;
                float window_height_;

                float draw_height_;
                float draw_width_;
        };
    }
}
 