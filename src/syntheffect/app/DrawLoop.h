#pragma once

#include "syntheffect/patch/Patch.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/video/Video.h"

#include <vector> 

namespace syntheffect {
    namespace app {
        class DrawLoop {
            public:
                DrawLoop();
                bool load(std::string patch_path, std::string video_path);
                std::shared_ptr<patch::Patch> patch;
                std::shared_ptr<graphics::PingPongBufferMap> channels;
                bool update(std::shared_ptr<graphics::Params> effect_params, float time);
                bool isDrawable();
                void seek(int relative_frames);

                float getWidth();
                float getHeight();
                float getFPS();
                std::shared_ptr<video::Video> video;

            private:
                bool drawable_;
        };
    }
}
 
