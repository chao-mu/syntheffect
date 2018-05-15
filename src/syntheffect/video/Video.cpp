#include "syntheffect/video/Video.h"

#include "ofGraphics.h"

namespace syntheffect {
    namespace video {
        bool Video::load(std::string path) {
            if (!video_player_.load(path)) {
                return false;
            }

            video_player_.setLoopState(OF_LOOP_NONE);
            //video_player_.setVolume(0);
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

        void Video::seek(int relative_frames) {
            int current = video_player_.getCurrentFrame();
            // Will be negative if we haven't done an update yet
            if (current < 0) {
                return;
            }

            int seek_to = relative_frames + current;
            if (seek_to < 0) {
                seek_to += video_player_.getTotalNumFrames();
            }

            video_player_.setFrame(seek_to % video_player_.getTotalNumFrames());
        }

        bool Video::isFrameNew() {
            return last_drawn_frame_ < video_player_.getCurrentFrame();
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
