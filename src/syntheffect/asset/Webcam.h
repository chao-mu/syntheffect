#pragma once

#include "ofVideoGrabber.h"

#include "syntheffect/asset/Drawable.h"

namespace syntheffect {
    namespace asset {
        class Webcam : public Drawable {
            public:
                Webcam(const std::string& path, int device_id);
                void setup() override;
                void update(float t) override;
                void draw(float x, float y, float width, float height) override;
                float getWidth() override;
                float getHeight() override;
                bool isReady() override;
                bool isFrameNew() override;

            private:
                ofVideoGrabber grabber_;
                int device_id_;
        };
    }
}
