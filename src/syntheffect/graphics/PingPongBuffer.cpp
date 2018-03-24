#include "syntheffect/graphics/PingPongBuffer.h"

#include "ofGraphics.h"

namespace syntheffect {
    namespace graphics {
        PingPongBuffer::PingPongBuffer() : src(make_shared<ofFbo>()), dest(make_shared<ofFbo>()) {
            fbos_.push_back(src);
            fbos_.push_back(dest);
        }

        void PingPongBuffer::allocate(int width, int height, int internal_format) {
            for (auto fbo: fbos_) {
                fbo->allocate(width, height, internal_format);

                // Set magnification/minification algorithm
                fbo->getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

                // Clear any lingering garbage
                fbo->begin();
                ofClear(255, 255, 255, 255);
                fbo->end();
            }
        }

        float PingPongBuffer::getWidth() {
            return src->getWidth();
        }

        float PingPongBuffer::getHeight() {
            return src->getHeight();
        }

        bool PingPongBuffer::isAllocated() {
            return src->isAllocated() && dest->isAllocated();
        }

        void PingPongBuffer::swap() {
            src.swap(dest);
        }
    }
}