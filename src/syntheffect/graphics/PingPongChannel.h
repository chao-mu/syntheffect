#pragma once

#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/graphics/Drawable.h"

namespace syntheffect {
    namespace graphics {
        class PingPongChannel : public Drawable {
            public:
                PingPongChannel(std::shared_ptr<graphics::PingPongBuffer> buf);
                void draw(float x, float y, float width, float height) override;
                float getWidth() override;
                float getHeight() override;

            private:
                std::shared_ptr<graphics::PingPongBuffer> buf_;
        };
    }
}
