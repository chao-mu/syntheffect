#pragma once

#include "ofVideoPlayer.h"

#include "syntheffect/asset/Drawable.h"

namespace syntheffect {
    namespace asset {
        class Video : public Drawable {
           public:
                Video(const std::string& name, const std::string& path, float volume, ofLoopType loop=OF_LOOP_NONE);
                void setup() override;
                void update(float t) override;
                void draw(float x, float y, float width, float height) override;
                float getWidth() override;
                float getHeight() override;
                bool isReady() override;
                bool isFrameNew() override;

                void restart() override;
                void play() override;
                void pause() override;

            private:
                std::string path_;
                float volume_;
                ofVideoPlayer player_;
                ofLoopType loop_;
        };
    }
}
