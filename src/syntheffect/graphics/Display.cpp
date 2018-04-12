#include "syntheffect/graphics/Display.h"

#include "ofGraphics.h"
#include "ofUtils.h"

namespace syntheffect {
    namespace graphics {
        Display::Display() {
        }

        void Display::allocate(float width, float height, int window_width, int window_height) {
            width_ = width;
            height_ = height;
            window_width_ = window_width;
            window_height_ = window_height;

            last_texture_.allocate(width_, height_, GL_RGBA);
            buffer_copy_.allocate(width_ * height_ * 4, GL_STATIC_DRAW);

            setupDrawSize();
        }

        bool Display::isAllocated() {
            return last_texture_.isAllocated() && buffer_copy_.isAllocated();
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

        ofTexture Display::getLastTexture() {
            return last_texture_;
        }

        void Display::draw(shared_ptr<graphics::PingPongBuffer> ping_pong) {
            ofSetColor(255);
            ping_pong->dest->draw(
                (window_width_ / 2.0) - (draw_width_ / 2.0),
                (window_height_ / 2.0) - (draw_height_ / 2.0),
                draw_width_, draw_height_);

            ping_pong->dest->getTexture().copyTo(buffer_copy_);
            last_texture_.loadData(buffer_copy_, GL_RGBA, GL_UNSIGNED_BYTE);
        }
    }
}