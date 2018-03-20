#include "syntheffect/ofApp.h"
#include "syntheffect/midi/MidiMessage.h"

namespace syntheffect {
    ofApp::ofApp(RtMidiIn* midi_in) : ofBaseApp() {
        midi_in_ = midi_in;
    }

    void ofApp::setup(){
        video_player_.load("rtp://127.0.0.1:1234");
        video_player_.setUseTexture(true);
        video_player_.setVolume(0);
        video_player_.play();

        video_width_ = video_player_.getWidth();
        video_height_ = video_player_.getHeight();

        delay_filter_ = filter::Delay();

        setupPlane(video_width_, video_height_);
        setupFbo(video_width_, video_height_);
        setupDrawSize(fbo_);
    }

    void ofApp::setupDrawSize(ofFbo fbo) {
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

    void ofApp::setupPlane(int w, int h) {
        plane_.set(w, h);
        plane_.setPosition(w/2, h/2, 0);
        plane_.mapTexCoords(0, 0, w, h);
    }

    void ofApp::setupFbo(int w, int h) {
        fbo_.allocate(w, h);
        fbo_.begin();
        ofClear(255, 255, 255, 255);
        fbo_.end();
    }

    void ofApp::update() {
        video_player_.update();
        std::vector<unsigned char> raw_message;
        while (midi_in_->getMessage(&raw_message) > 0) {
            midi::MidiMessage msg(raw_message);
            handleCmdMicroEvent(msg);
        }
    }

    void ofApp::draw() {
        ofTexture tex = video_player_.getTexture();
        ofTexture last_tex = fbo_.getTexture();
        if (!tex.isAllocated() || !last_tex.isAllocated()) {
            return;
        }

        float t = ofGetElapsedTimef();

        delay_filter_.setLastTexture(last_tex);

        tex.bind();
        fbo_.begin();
        delay_filter_.begin(video_width_, video_height_, t);

        plane_.draw();

        delay_filter_.end();
        fbo_.end();
        tex.unbind();

        ofSetColor(255);
        fbo_.draw(
            (ofGetWidth() / 2.0) - (draw_width_ / 2.0),
            (ofGetHeight() / 2.0) - (draw_height_ / 2.0),
            draw_width_, draw_height_);
    }

    void ofApp::onCmdMicroLeftLeftFaderSlide(unsigned char v) {
        delay_filter_.start();
        delay_filter_.setIntensity((float)v, 0, 127);
    }

    void ofApp::windowResized(int w, int h) {
        setupDrawSize(fbo_);
    }
}