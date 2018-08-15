#pragma once

#include "ofVideoGrabber.h"

#include "syntheffect/graphics/Drawable.h"
#include "syntheffect/param/Params.h"

namespace syntheffect {
    namespace graphics {
        class Webcam : public Drawable {
            public:
                void setup() override;
                void update(float t, param::Params& params) override;
                void draw(float x, float y, float width, float height) override;
                float getWidth() override;
                float getHeight() override;
                bool isReady() override;

            private:
                ofVideoGrabber grabber_;
        };
    }
}
