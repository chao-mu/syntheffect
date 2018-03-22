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
            video_player_.setVolume(0);
            video_player_.play();

            video_width_ = video_player_.getWidth();
            video_height_ = video_player_.getHeight();

            setupPlane(video_width_, video_height_);
            setupFbo(video_width_, video_height_);
            setupDrawSize(fbo_);

            return true;
        }

        bool Video::update() {
            video_player_.update();
            return !video_player_.getIsMovieDone();
        }

        void Video::windowResized(int w, int h) {
            setupDrawSize(fbo_);
        }

        void Video::setupDrawSize(ofFbo fbo) {
            float aspect = fbo.getWidth() / fbo.getHeight();
            float window_aspect = ofGetWidth() / ofGetHeight();
            float scale_factor;

            if (window_aspect > aspect) {
                scale_factor = ofGetHeight() / fbo.getHeight();
            } else {
                scale_factor = ofGetWidth() / fbo.getWidth();
            }

            draw_height_ = fbo.getHeight() * scale_factor;
            draw_width_ = fbo.getWidth() * scale_factor;
        }

        void Video::setupPlane(int w, int h) {
            plane_.set(w, h);
            plane_.setPosition(w/2, h/2, 0);
            plane_.mapTexCoords(0, 0, w, h);
        }

        void Video::setupFbo(int w, int h) {
            fbo_.allocate(w, h);
            fbo_.begin();
            ofClear(255, 255, 255, 255);
            fbo_.end();
        }

        ofTexture Video::getLastTexture() {
            return fbo_.getTexture();
        }

        bool Video::isAllocated() {
            return fbo_.getTexture().isAllocated() && video_player_.getTexture().isAllocated();
        }

        void Video::draw(shared_ptr<filter::FilterChain> filters) {
            float t = ofGetElapsedTimef();
            ofTexture tex = video_player_.getTexture();

            tex.bind();
            fbo_.begin();
            filters->begin(video_width_, video_height_, t);

            plane_.draw();

            filters->end();
            fbo_.end();
            tex.unbind();
            

            ofSetColor(255);
            fbo_.draw(
                (ofGetWidth() / 2.0) - (draw_width_ / 2.0),
                (ofGetHeight() / 2.0) - (draw_height_ / 2.0),
                draw_width_, draw_height_);
        }
    }
}