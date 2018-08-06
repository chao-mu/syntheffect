#include "syntheffect/graphics/PingPongChannel.h"

namespace syntheffect {
    namespace graphics {
        PingPongChannel::PingPongChannel(std::shared_ptr<graphics::PingPongBuffer> buf) : Drawable() {
            buf_ = buf;
        }

        void PingPongChannel::draw(float x, float y, float width, float height) {
            buf_->drawable()->draw(x, y, width, height);
        }

        float PingPongChannel::getHeight() {
            return buf_->getHeight();
        }

        float PingPongChannel::getWidth() {
            return buf_->getWidth();
        }
    }
}
