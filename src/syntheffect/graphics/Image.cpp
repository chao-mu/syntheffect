#include "syntheffect/graphics/Image.h"

#include <stdexcept>

namespace syntheffect {
    namespace graphics {
        Image::Image(std::string path) : Drawable() {
            path_ = path;
        }

        void Image::setup() {
            image_.setUseTexture(true);
            if (!image_.load(path_)) {
                throw std::runtime_error("Error loading video with path " + path_);
            }
        }

        void Image::draw(float x, float y, float width, float height) {
            image_.draw(x, y, width, height);
        }

        float Image::getHeight() {
            return image_.getHeight();
        }

        float Image::getWidth() {
            return image_.getWidth();
        }
    }
}