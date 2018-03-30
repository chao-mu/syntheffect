#pragma once

#include "ofFbo.h"
#include "ofVideoPlayer.h"
#include "of3dPrimitives.h"

#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/patch/Patch.h"

namespace syntheffect {
    namespace video {
        class Video {
            public:
                bool load(std::string path);
                void draw(shared_ptr<patch::Patch> patch);
                void windowResized(int w, int h);
                bool update();
                ofTexture getLastTexture();
                bool isAllocated();

            private:
                ofPlanePrimitive plane_;
                ofVideoPlayer video_player_;

                ofBufferObject buffer_copy_;
                ofTexture last_texture_;

                graphics::PingPongBuffer ping_pong_;

                void setupDrawSize(float w, float h);

                int draw_height_;
                int draw_width_;
        };
    }
}