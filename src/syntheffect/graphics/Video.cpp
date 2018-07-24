#include "syntheffect/graphics/Video.h"

#include <stdexcept>

namespace syntheffect {
    namespace graphics {
        Video::Video(std::string path, float volume) : Drawable() {
            path_ = path;
            volume_ = volume;
        }

        void Video::setup() {
            video_.setUseTexture(true);
            video_.setLoopState(OF_LOOP_NORMAL);
            if (!video_.load(path_)) {
                throw std::runtime_error("Error loading video with path " + path_);
            }
            video_.setVolume(volume_);
            video_.play();
        }

        bool Video::update(std::shared_ptr<param::Params> params, float t) {
            video_.update();
            return true;
        }

        void Video::draw(float x, float y, float width, float height) {
            video_.draw(x, y, width, height);
        }

        bool Video::isReady() {
            return video_.isInitialized();
        }

        float Video::getHeight() {
            return video_.getHeight();
        }

        float Video::getWidth() {
            return video_.getWidth();
        }
    }
}
