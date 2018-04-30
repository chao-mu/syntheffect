#pragma once

#include "ofVideoPlayer.h"

#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace video {
        class Video {
            public:
                bool load(std::string path);
                void drawTo(shared_ptr<graphics::PingPongBuffer> ping_pong_);
                bool update();
                bool isAllocated();
                float getWidth();
                float getHeight();
                void seek(int relative_frames);
                float getFPS();
                bool isFrameNew();

            private:
                ofVideoPlayer video_player_;
        };
    }
}