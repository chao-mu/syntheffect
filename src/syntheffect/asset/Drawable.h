#ifndef SYNTHEFFECT_ASSET_DRAWABLE_H
#define SYNTHEFFECT_ASSET_DRAWABLE_H

#pragma once

#include <memory>

#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/asset/Asset.h"

namespace syntheffect {
    namespace asset {
        class Drawable : public Asset {
            public:
                Drawable(std::string id);

                virtual void draw(float x, float y, float width, float height) = 0;
                virtual float getWidth() = 0;
                virtual float getHeight() = 0;
                virtual bool isFrameNew();

                void drawScaleCenter(float dest_w, float dest_h);
        };
    }
}

#endif
