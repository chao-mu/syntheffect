#pragma once

#include "ofBufferObject.h"
#include "syntheffect/graphics/PingPongBufferMap.h"

namespace syntheffect {
    namespace graphics {
        class Display {
            public:
                Display();
                void load(float width, float height, int window_width, int window_height);
                void windowResized(int window_width, int window_height);
                void draw(std::shared_ptr<graphics::PingPongBufferMap> buffers, std::vector<std::string>);
                void draw(std::shared_ptr<ofFbo> buffer);
                void setupDrawSize();
                std::shared_ptr<ofFbo> make_tiled(std::shared_ptr<PingPongBufferMap> buffers, std::vector<std::string> keys);

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
 
