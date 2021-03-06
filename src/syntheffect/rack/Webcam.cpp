#include "syntheffect/rack/Webcam.h"

#include <stdexcept>

#include "syntheffect/graphics/DrawInfo.h"

namespace syntheffect {
    namespace rack {
        Webcam::Webcam(const std::string& id) : Module(id) {
        }

        const std::string Webcam::getType() {
            return getModuleType();
        }

        const std::string Webcam::getModuleType() {
            return "builtin/webcam";
        }

        void Webcam::setup(int width, int height, int internal_format, const std::string& workspace_dir) {
            player_.setUseTexture(true);

            if (!player_.setup(width, height)) {
                throw std::runtime_error("Error loading webcam");
            }

            outputs_.allocate(width, height, internal_format);

            ofTexture& tex = outputs_.getTexture();

            draw_info_ = graphics::DrawInfo::scaleCenter(player_.getWidth(), player_.getHeight(), width, height);

            auto red = std::make_shared<Channel>(tex, 0);
            auto green = std::make_shared<Channel>(tex, 1);
            auto blue = std::make_shared<Channel>(tex, 2);

            output_channels_["red"] = red;
            output_channels_["green"] = green;
            output_channels_["blue"] = blue;
            output_channels_["frame_new"] = std::make_shared<Channel>(1);
        }

        void Webcam::update(float /* t */) {
            player_.update();

            output_channels_["frame_new"]->value_ = player_.isFrameNew() ? 1 : 0;

            outputs_.begin();
            player_.draw(
                draw_info_.x_,
                draw_info_.y_,
                draw_info_.w_,
                draw_info_.h_
            );
            outputs_.end();
        }

        bool Webcam::isReady() {
            return player_.isInitialized();
        }
    }
}
