#include "syntheffect/graphics/Display.h"

#include "ofGraphics.h"
#include "ofUtils.h"

namespace syntheffect {
    namespace graphics {
        Display::Display() {
        }

        void Display::load(float width, float height, int window_width, int window_height) {
            width_ = width;
            height_ = height;
            window_width_ = window_width;
            window_height_ = window_height;

            setupDrawSize();
        }

        void Display::setupDrawSize() {
            float w = width_;
            float h = height_;

            float aspect = w / h;
            float window_aspect = window_width_ / window_height_;
            float scale_factor;

            if (window_aspect > aspect) {
                scale_factor = window_height_ / h;
            } else {
                scale_factor = window_width_ / w;
            }

            draw_height_ = h * scale_factor;
            draw_width_ = w * scale_factor;
        }

        void Display::windowResized(int window_width, int window_height) {
            window_width_ = window_width;
            window_height_ = window_height;
            setupDrawSize();
        }

        void Display::draw(shared_ptr<graphics::PingPongBuffer> ping_pong) {
            ofSetColor(255);
            shared_ptr<ofFbo> buffer = ping_pong->drawable();

            buffer->draw(
                (window_width_ / 2.0) - (draw_width_ / 2.0),
                (window_height_ / 2.0) - (draw_height_ / 2.0),
                draw_width_, draw_height_);
        }
    }
}