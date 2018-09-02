#ifndef SYNTHEFFECT_ASSET_ASSET_H
#define SYNTHEFFECT_ASSET_ASSET_H

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
                void setID(std::string id);

                bool isActive();
                void setActive(bool active);


                std::string getStack();
                void setStack(std::string stack);

                std::string getGroup();
                void setGroup(std::string stack);

            private:
                std::string id_;
                std::string stack_;
                std::string group_;
                bool active_;
        };
    }
}

#endif
