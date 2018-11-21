#include "syntheffect/rack/Webcam.h"

#include <stdexcept>

#include "syntheffect/graphics/Util.h"

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

            ofTexture& tex = player_.getTexture();

            auto red = std::make_shared<Channel>(tex, 0);
            auto green = std::make_shared<Channel>(tex, 1);
            auto blue = std::make_shared<Channel>(tex, 2);

            output_channels_["red"] = red;
            output_channels_["r"] = red;

            output_channels_["green"] = green;
            output_channels_["g"] = green;

            output_channels_["blue"] = blue;
            output_channels_["b"] = blue;

            output_channels_["frame_new"] = std::make_shared<Channel>(1);
        }

        void Webcam::update(float /* t */) {
            player_.update();

            output_channels_["frame_new"]->value_ = player_.isFrameNew() ? 1 : 0;

            outputs_.begin();
            graphics::Util::drawScaleCenter(player_.getWidth(), player_.getHeight(), outputs_.getWidth(), outputs_.getHeight(),
                    [this](float x, float y, float w, float h) { player_.draw(x, y, w, h); });
            outputs_.end();
        }

        bool Webcam::isReady() {
            return player_.isInitialized();
        }
    }
}
