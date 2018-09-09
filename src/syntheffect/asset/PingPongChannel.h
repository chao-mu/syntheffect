#pragma once

#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/asset/Drawable.h"

namespace syntheffect {
    namespace asset {
        class PingPongChannel : public Drawable {
            public:
                PingPongChannel(const std::string& name, std::shared_ptr<graphics::PingPongBuffer> buf);
                void draw(float x, float y, float width, float height) override;
                float getWidth() override;
                float getHeight() override;

            private:
                std::shared_ptr<graphics::PingPongBuffer> buf_;
        };
    }
}
