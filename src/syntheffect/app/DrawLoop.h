#pragma once

#include <vector>

#include "ofVideoPlayer.h"

#include "syntheffect/patch/Patch.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/graphics/Video.h"

namespace syntheffect {
    namespace app {
        class DrawLoop {
            public:
                DrawLoop();
                bool load(std::string patch_path, std::string video_path);
                std::shared_ptr<patch::Patch> patch;
                std::shared_ptr<graphics::PingPongBufferMap> channels;
                bool update(std::shared_ptr<graphics::Params> effect_params, float time);
                bool isReady();
                void seek(int relative_frames);

                float getWidth();
                float getHeight();
                float getFPS();
                graphics::Video video;
                void play();
                void stop();
                void setLoopState(ofLoopType state);

            private:
                bool ready_;
        };
    }
}
