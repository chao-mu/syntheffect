#pragma once

#include "ofImage.h"

#include "syntheffect/graphics/Drawable.h"

namespace syntheffect {
    namespace graphics {
        class Image : public Drawable {
            public:
                Image(std::string path);
                void setup() override;
                void draw(float x, float y, float width, float height) override;
                float getWidth() override;
                float getHeight() override;

            private:
                std::string path_;
                ofImage image_;
        };
    }
}
