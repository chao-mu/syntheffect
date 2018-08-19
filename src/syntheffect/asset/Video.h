#pragma once

#include "ofVideoPlayer.h"

#include "syntheffect/asset/Asset.h"

namespace syntheffect {
    namespace asset {
        class Video : public Asset {
            public:
                Video(std::string name, std::string path, float volume);
                void setup() override;
                void update(float t) override;
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
