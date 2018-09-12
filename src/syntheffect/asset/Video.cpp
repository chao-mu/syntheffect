#include "syntheffect/asset/Video.h"

#include <stdexcept>

namespace syntheffect {
    namespace asset {
        Video::Video(const std::string& id, const std::string& path, float volume, ofLoopType loop) :
            Drawable(id),
            path_(path),
            volume_(volume),
            loop_(loop) {}

        void Video::setup() {
            player_.setUseTexture(true);
            if (!player_.load(path_)) {
                throw std::runtime_error("Error loading video with path " + path_);
            }

            player_.setLoopState(loop_);
            player_.setVolume(volume_);
        }

        bool Video::isFrameNew() {
            return player_.isFrameNew();
        }

        void Video::update(float t) {
            player_.update();
        }

        void Video::draw(float x, float y, float width, float height) {
            player_.draw(x, y, width, height);
        }

        bool Video::isReady() {
            return player_.isInitialized();
        }

        float Video::getHeight() {
            return player_.getHeight();
        }

        float Video::getWidth() {
            return player_.getWidth();
        }

        void Video::restart() {
            player_.setPosition(0);
        }

        void Video::play() {
            player_.play();
        }

        void Video::pause() {
            player_.setPaused(true);
        }
    }
}
