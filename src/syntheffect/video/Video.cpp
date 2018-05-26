#include "syntheffect/video/Video.h"

#include "ofGraphics.h"

namespace syntheffect {
    namespace video {
        bool Video::load(std::string path) {
            video_player_.setUseTexture(true);

            if (!video_player_.load(path)) {
                return false;
            }

            video_player_.setLoopState(OF_LOOP_NONE);
            video_player_.play();

            return true;
        }

        float Video::getFPS() {
            return video_player_.getTotalNumFrames() / video_player_.getDuration();
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

        void Video::nextFrame() {
            ofLogNotice() << std::to_string(video_player_.getCurrentFrame());
            video_player_.nextFrame();
        }

        void Video::drawTo(std::shared_ptr<graphics::PingPongBuffer> ping_pong) {
            last_drawn_frame_ = video_player_.getCurrentFrame();
            ping_pong->begin();
            ofClear(255, 255, 255, 0);
            video_player_.draw(0, 0);
            ping_pong->end();
        }
    }
}
