#include "syntheffect/ofApp.h"

#include "syntheffect/midi/MidiMessage.h"
#include "syntheffect/patch/PatchBuilder.h"

#define CHANNEL_ONE "channel1"
#define CHANNEL_OUT "out"

#define DEBUG_CHANNEL_ACCESS true

namespace syntheffect {
    ofApp::ofApp(shared_ptr<RtMidiIn> midi_in, std::string playlist_path) 
            : ofBaseApp(),
            midi_in_(move(midi_in)) {
        playlist_path_ = playlist_path;
    }

    void ofApp::setup() {
        playlist_.load(playlist_path_);

        ofSetBackgroundAuto(true);

        nextVideo();
    }

    void ofApp::nextVideo() {
        video_ = playlist_.next();
        float w = video_.getWidth();
        float h = video_.getHeight();

        display_ = graphics::Display();
        display_.allocate(w, h, ofGetWidth(), ofGetHeight());

        buffers_ = make_shared<graphics::PingPongBufferMap>(w, h);

        buffers_->allocate(CHANNEL_ONE);
        buffers_->allocate(CHANNEL_OUT);

        unique_ptr<patch::PatchBuilder> builder = make_unique<patch::PatchBuilder>();

        patch_ = builder->build("patches/default.xml", buffers_);
        if (!patch_) {
            throw runtime_error("Unable to build patch!");
        }
    }

    void ofApp::update() {
        if (!video_.update()) {
            nextVideo();
            video_.update();
        }

        std::vector<unsigned char> raw_message;
        while (midi_in_->getMessage(&raw_message) > 0) {
            midi::MidiMessage msg(raw_message);
            handleCmdMicroEvent(msg);
        }
    }

    void ofApp::windowResized(int w, int h) {
        display_.windowResized(w, h);
    }

    void ofApp::draw() {
        if (!video_.isAllocated()) {
            return;
        }

        video_.draw(buffers_->get(CHANNEL_ONE));
        patch_->draw(buffers_, ofGetElapsedTimef());
        display_.draw(buffers_->get(CHANNEL_OUT));

        if (DEBUG_CHANNEL_ACCESS) {
            for (auto& kv : buffers_->getAccessHistory()) {
                if (!kv.second) {
                    ofLogWarning() << "Unaccess channel: " + kv.first;
                }
            }
        }

        std::stringstream strm;
        strm << "fps: " << ofGetFrameRate();
        ofSetWindowTitle(strm.str());
    }

    void ofApp::keyPressed(int c) {
        if (c == 'p') {
            ofPixels pixels;
            display_.getLastTexture().readToPixels(pixels);

            ofImage image;
            image.setFromPixels(pixels);
            image.save("out-" + ofGetTimestampString() + ".png");
        }
    }

    void ofApp::onCmdMicroLeftLeftFaderSlide(unsigned char v) {
        // delay_filter_->start();
        // delay_filter_->setIntensity((float)v, 0, 127);
    }
}

#undef CHANNEL_ONE
#undef CHANNEL_OUT
#undef DEBUG_CHANNEL_ACCESS