#pragma once

#include "ofVideoPlayer.h"

#include "syntheffect/graphics/Drawable.h"

namespace syntheffect {
    namespace graphics {
        class Video : public Drawable {
            public:
                Video(std::string path, float volume);
                void setup() override;
                void update(float t, param::Params& params) override;
                void draw(float x, float y, float width, float height) override;
                float getWidth() override;
                float getHeight() override;
                bool isReady() override;

            private:
                std::string path_;
                float volume_;
                ofVideoPlayer video_;
        };
    }
}
