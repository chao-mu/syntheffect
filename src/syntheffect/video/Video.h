#pragma once

#include "ofVideoPlayer.h"

#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace video {
        class Video {
            public:
                bool load(std::string path);
                void draw(shared_ptr<graphics::PingPongBuffer> ping_pong_);
                bool update();
                bool isAllocated();
                float getWidth();
                float getHeight();

            private:
                ofVideoPlayer video_player_;
        };
    }
}