#pragma once

#include <memory>

#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace asset {
        class Asset {
            public:
                Asset(std::string id);

                virtual void setup();
                virtual void update(float t);
                virtual void draw(float x, float y, float width, float height) = 0;
                virtual float getWidth() = 0;
                virtual float getHeight() = 0;
                virtual bool isReady();
                virtual bool isFinished();
                virtual bool isFrameNew();

                void drawScaleCenter(float dest_w, float dest_h);

                std::string getID();
                std::string setID(std::string id);

            private:
                std::string id_;
        };
    }
}
