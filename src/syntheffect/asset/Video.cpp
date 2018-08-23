#include "syntheffect/asset/Video.h"

#include <stdexcept>

#include "ofLog.h"

namespace syntheffect {
    namespace asset {
        Video::Video(std::string name, std::string path, float volume, ofLoopType loop) : Asset(name) {
            path_ = path;
            volume_ = volume;
            loop_ = loop;
        }

        void Video::setup() {
            video_.setUseTexture(true);
            if (!video_.load(path_)) {
                throw std::runtime_error("Error loading video with path " + path_);
            }
            ofLog() << path_ << " " << std::to_string(loop_);
            video_.setLoopState(loop_);
            video_.setVolume(volume_);
            video_.play();
        }

        void Video::update(float t) {
            video_.update();
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
