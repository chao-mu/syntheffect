#pragma once

#include "ofImage.h"

#include "syntheffect/asset/Asset.h"

namespace syntheffect {
    namespace asset {
        class Image : public Asset {
            public:
                Image(std::string name, std::string path);
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
