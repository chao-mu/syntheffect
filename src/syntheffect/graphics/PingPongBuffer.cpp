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
                ofClear(0);
                fbo->end();
            }
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

        ofFbo& PingPongBuffer::source() {
            return *src_.get();
        }

        ofFbo& PingPongBuffer::dest() {
            return *dest_.get();
        }

        void PingPongBuffer::swap() {
            source().begin();
            dest().draw(0, 0);
            source().end();

            src_.swap(dest_);
        }
    }
}
