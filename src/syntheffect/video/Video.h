#pragma once

#include "ofFbo.h"
#include "ofVideoPlayer.h"
#include "of3dPrimitives.h"

#include "syntheffect/filter/FilterChain.h"

namespace syntheffect {
    namespace video {
        class Video {
            public:
                bool load(std::string path);
                void draw(filter::FilterChain filters);
                void windowResized(int w, int h);
                bool update();
                ofTexture getLastTexture();
                bool isAllocated();

            private:
                ofPlanePrimitive plane_;
                ofFbo fbo_;
                ofVideoPlayer video_player_;

                void setupPlane(int w, int h);
                void setupFbo(int w, int h);
                void setupDrawSize(ofFbo fbo);

                int draw_height_;
                int draw_width_;

                int video_width_;
                int video_height_;
        };
    }
}