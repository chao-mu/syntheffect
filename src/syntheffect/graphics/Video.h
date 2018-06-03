#pragma once

#include "ofVideoPlayer.h"

#include "syntheffect/graphics/Drawable.h"

namespace syntheffect {
    namespace graphics {
        class Video : public ofVideoPlayer, public Drawable {
            public:
                Video();
                void update(std::shared_ptr<graphics::Params> params, float t) override;
                bool load(std::string video_path) override;
                void draw() override;
                bool isReady() override;

            private:
                std::string path_;
        };
    }
}
