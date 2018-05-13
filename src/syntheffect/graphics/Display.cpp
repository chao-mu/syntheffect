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

        void Display::draw(std::shared_ptr<PingPongBufferMap> buffers, std::vector<std::string> keys) {
            float count = keys.size();
            assert(count <= 4 && count > 0);
            if (count == 1) {
                draw(buffers->get(keys[0])->drawable());
                return;
            }

            ofClear(255, 255, 255, 255);
            ofSetColor(255);

            int col = 0;
            int row = 0;
            int i = 0;
            float scale = 0.5;
            for (auto key : keys) {
                buffers->get(key)->drawable()->draw(
                    (draw_width_ * col) * scale,
                    (draw_height_ * row) * scale,
                    draw_width_ * scale,
                    draw_height_ * scale 
                );

                i++;
                col++;
                if (i == 2) {
                    col = 0;
                    row += 1;
                }
            }
        }

        void Display::draw(std::shared_ptr<ofFbo> buffer) {
            ofClear(255, 255, 255, 255);
            ofSetColor(255);

            buffer->draw(
                (window_width_ / 2.0) - (draw_width_ / 2.0),
                (window_height_ / 2.0) - (draw_height_ / 2.0),
                draw_width_, draw_height_);
        }
    }
}
