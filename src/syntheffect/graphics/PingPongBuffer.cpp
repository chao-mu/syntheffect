#include "syntheffect/graphics/PingPongBuffer.h"

#include "ofGraphics.h"

namespace syntheffect {
    namespace graphics {
        PingPongBuffer::PingPongBuffer() : src_(make_shared<ofFbo>()), dest_(make_shared<ofFbo>()) {
            receiving_ = false;
        }

        void PingPongBuffer::allocate(int width, int height, int internal_format) {
            vector<shared_ptr<ofFbo>> fbos = {src_, dest_};
            for (auto fbo: fbos) {
                fbo->allocate(width, height, internal_format);

                // Set magnification/minification algorithm
                fbo->getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

                // Clear any lingering garbage
                fbo->begin();
                ofClear(255, 255, 255, 255);
                fbo->end();
            }
        }

        void PingPongBuffer::drawTo(shared_ptr<PingPongBuffer> buff, int x, int y) {
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

        shared_ptr<ofFbo> PingPongBuffer::drawable() {
            return receiving_ ? src_ : dest_;
        }

        void PingPongBuffer::swap() {
            src_.swap(dest_);
        }
    }
}