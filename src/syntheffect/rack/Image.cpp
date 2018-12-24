#include "syntheffect/rack/Image.h"

#include <stdexcept>

namespace syntheffect {
    namespace rack {
        Image::Image(const std::string& id, const std::string& path) : Module(id), path_(path) { }

        const std::string Image::getType() {
            return getModuleType();
        }

        const std::string Image::getModuleType() {
            return "builtin/image";
        }

        void Image::setup(int width, int height, int internal_format, const std::string& /* workspace_dir */) {
            image_.setUseTexture(true);


            if (!image_.load(path_)) {
                throw std::runtime_error("Error loading image with path " + path_);
            }

            outputs_.allocate(width, height, internal_format);

            draw_info_ = graphics::DrawInfo::scaleCenter(image_.getWidth(), image_.getHeight(), width, height);

            ofTexture& tex = outputs_.getTexture();

            auto red = std::make_shared<Channel>(tex, 0);
            auto green = std::make_shared<Channel>(tex, 1);
            auto blue = std::make_shared<Channel>(tex, 2);
            auto alpha = std::make_shared<Channel>(tex, 3);

            output_channels_["red"] = red;
            output_channels_["green"] = green;
            output_channels_["blue"] = blue;
            output_channels_["alpha"] = alpha;
        }

        void Image::update(float /* t */) {
            outputs_.begin();
            image_.draw(
                draw_info_.x_,
                draw_info_.y_,
                draw_info_.w_,
                draw_info_.h_
            );
            outputs_.end();
        }
    }
}
