#include "syntheffect/video/Video.h"

#include "ofGraphics.h"

namespace syntheffect {
    namespace video {
        bool Video::load(std::string path) {
            if (!video_player_.load(path)) {
                return false;
            }

            video_player_.setUseTexture(true);
            video_player_.setLoopState(OF_LOOP_NONE);
            video_player_.setVolume(0);
            video_player_.play();

            return true;
        }

        bool Video::update() {
            video_player_.update();
            return !video_player_.getIsMovieDone();
        }

        bool Video::isAllocated() {
            return video_player_.getTexture().isAllocated();
        }

        float Video::getWidth() {
            return video_player_.getWidth();
        }

        float Video::getHeight() {
            return video_player_.getHeight();
        }

        void Video::draw(shared_ptr<graphics::PingPongBuffer> ping_pong) {
            ping_pong->begin();
            ofClear(255, 255, 255, 0);
            video_player_.draw(0, 0);
            ping_pong->end();
        }
    }
}