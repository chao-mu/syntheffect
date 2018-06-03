#include "syntheffect/graphics/PingPongBuffer.h"

#include "ofGraphics.h"

namespace syntheffect {
    namespace graphics {
        PingPongBuffer::PingPongBuffer() : src_(std::make_shared<ofFbo>()), dest_(std::make_shared<ofFbo>()) {
            receiving_ = false;
        }

        void PingPongBuffer::allocate(int width, int height, int internal_format) {
            for (auto fbo: {src_, dest_}) {
                fbo->allocate(width, height, internal_format);

                // Set magnification/minification algorithm
                fbo->getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

                // Clear any lingering garbage
                fbo->begin();
                ofClear(255, 255, 255, 255);
                fbo->end();
            }
        }

        void PingPongBuffer::drawTo(std::shared_ptr<PingPongBuffer> buff, int x, int y) {
            buff->begin();
            drawable()->draw(x, y);
            buff->end();
        }

        float PingPongBuffer::getWidth() {
            return src_->getWidth();
        }

        float PingPongBuffer::getHeight() {
            return src_->getHeight();
        }

        void PingPongBuffer::drawFrom(std::function<void()> draw) {
            begin();
            ofClear(255, 255, 255, 0);
            draw();
            end();
        }

        bool PingPongBuffer::isAllocated() {
            return src_->isAllocated() && dest_->isAllocated();
        }

        void PingPongBuffer::begin() {
            swap();
            receiving_ = true;
            dest_->begin();
        }

        void PingPongBuffer::end() {
            receiving_ = false;
            dest_->end();
        }

        std::shared_ptr<ofFbo> PingPongBuffer::drawable() {
            return receiving_ ? src_ : dest_;
        }

        void PingPongBuffer::swap() {
            src_.swap(dest_);
        }
    }
}
