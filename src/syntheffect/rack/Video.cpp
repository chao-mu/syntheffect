#include "syntheffect/rack/Video.h"

#include <stdexcept>

#include "syntheffect/graphics/Util.h"

namespace syntheffect {
    namespace rack {
        Video::Video(const std::string& id, const std::string& path) : Module(id), path_(path) {
        }

        void Video::setup(int width, int height, int internal_format) {
            player_.setUseTexture(true);

            if (!player_.load(path_)) {
                throw std::runtime_error("Error loading video with path " + path_);
            }

            player_.setLoopState(OF_LOOP_NORMAL);
            player_.setVolume(0);

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
        }

        void Video::start() {
            player_.play();
        }

        void Video::update(float /* t */) {
            player_.update();

            outputs_.begin();
            graphics::Util::drawScaleCenter(player_.getWidth(), player_.getHeight(), outputs_.getWidth(), outputs_.getHeight(),
                    [this](float x, float y, float w, float h) { player_.draw(x, y, w, h); });
            outputs_.end();
        }

        bool Video::isReady() {
            return player_.isInitialized();
        }
    }
}
