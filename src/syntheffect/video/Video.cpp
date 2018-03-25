#include "syntheffect/video/Video.h"

#include "ofUtils.h"
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

            float w = video_player_.getWidth();
            float h = video_player_.getHeight();

            ping_pong_.allocate(w, h);
            setupDrawSize(w, h);

            last_texture_.allocate(w, h, GL_RGBA);
            buffer_copy_.allocate(w * h * 4, GL_STATIC_DRAW);

            return true;
        }

        bool Video::update() {
            video_player_.update();
            return !video_player_.getIsMovieDone();
        }

        void Video::windowResized(int w, int h) {
            setupDrawSize(ping_pong_.getWidth(), ping_pong_.getHeight());
        }

        void Video::setupDrawSize(float w, float h) {
            float aspect = w / h;
            float window_aspect = ofGetWidth() / ofGetHeight();
            float scale_factor;

            if (window_aspect > aspect) {
                scale_factor = ofGetHeight() / h;
            } else {
                scale_factor = ofGetWidth() / w;
            }

            draw_height_ = h * scale_factor;
            draw_width_ = w * scale_factor;
        }

        ofTexture Video::getLastTexture() {
            return last_texture_;
        }

        bool Video::isAllocated() {
            return ping_pong_.isAllocated() && video_player_.getTexture().isAllocated();
        }

        void Video::draw(vector<shared_ptr<filter::FilterBase>> filters) {
            ping_pong_.dest->begin();
            ofClear(0, 255);
            video_player_.draw(0, 0);
            ping_pong_.dest->end();

            float t = ofGetElapsedTimef();
            for (auto filter: filters) {
                if (filter->isActive()) {
                    ping_pong_.swap();
                    filter->draw(ping_pong_, t);
                }
            }

            ofSetColor(255);
            ping_pong_.dest->draw(
                (ofGetWidth() / 2.0) - (draw_width_ / 2.0),
                (ofGetHeight() / 2.0) - (draw_height_ / 2.0),
                draw_width_, draw_height_);

            ping_pong_.dest->getTexture().copyTo(buffer_copy_);
            last_texture_.loadData(buffer_copy_, GL_RGBA, GL_UNSIGNED_BYTE);
        }
    }
}