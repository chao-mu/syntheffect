#pragma once

#include <vector>
#include <memory>

#include "ofVideoPlayer.h"

#include "syntheffect/patch/Patch.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/graphics/Drawable.h"

namespace syntheffect {
    namespace app {
        class Renderer : public graphics::Drawable {
            public:
                Renderer(std::string patch_path, std::vector<std::shared_ptr<graphics::Drawable>> drawables);
                void setup();

                bool update(std::shared_ptr<param::Params> effect_params, float time);
                bool isReady();
                void seek(int relative_frames);

                float getWidth();
                float getHeight();

                void setLoopState(ofLoopType state);

                std::shared_ptr<graphics::PingPongBufferMap> channels;

                void draw(float x, float y, float width, float height) override;

            private:
                bool ready_;
                std::string patch_path_;
                std::vector<std::shared_ptr<graphics::Drawable>> drawables_;

                std::string getChannelName(int i);
                std::string getLastName(int i);
                std::shared_ptr<patch::Patch> patch_;
        };
    }
}
