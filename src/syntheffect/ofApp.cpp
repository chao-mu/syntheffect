#include "syntheffect/ofApp.h"

#include "syntheffect/midi/MidiMessage.h"

namespace syntheffect {
    ofApp::ofApp(shared_ptr<RtMidiIn> midi_in, std::string playlist_path) 
            : ofBaseApp(),
            delay_filter_(make_shared<filter::Delay>()),
            hue_filter_(make_shared<filter::Huerific>()),
            spin_zoom_filter_(make_shared<filter::SpinZoom>()),
            color_displace_filter_(make_shared<filter::ColorDisplace>()),
            filters_(),
            midi_in_(move(midi_in)) {
        playlist_path_ = playlist_path;

        filters_.push_back(delay_filter_);
        filters_.push_back(hue_filter_);
        filters_.push_back(spin_zoom_filter_);
        filters_.push_back(color_displace_filter_);
    }

    void ofApp::setup() {
        playlist_.load(playlist_path_);

        video_ = playlist_.next();
        // color_displace_filter_->start();
        // spin_zoom_filter_->start();
        // delay_filter_->start();
        // hue_filter_->start();
        // delay_filter_->setIntensity(0.5, 0, 1);
    }

    void ofApp::update() {
        if (!video_.update()) {
            video_ = playlist_.next();
            video_.update();
        }

        std::vector<unsigned char> raw_message;
        while (midi_in_->getMessage(&raw_message) > 0) {
            midi::MidiMessage msg(raw_message);
            handleCmdMicroEvent(msg);
        }
    }

    void ofApp::draw() {
        if (!video_.isAllocated()) {
            return;
        }

        delay_filter_->setLastTexture(video_.getLastTexture());
        video_.draw(filters_);

        std::stringstream strm;
        strm << "fps: " << ofGetFrameRate();
        ofSetWindowTitle(strm.str());
    }

    void ofApp::keyPressed(int c) {
        if (c == 'p') {
            ofPixels pixels;
            video_.getLastTexture().readToPixels(pixels);

            ofImage image;
            image.setFromPixels(pixels);
            image.save("out-" + ofGetTimestampString() + ".png");
        }
    }

    void ofApp::onCmdMicroLeftLeftFaderSlide(unsigned char v) {
        delay_filter_->start();
        delay_filter_->setIntensity((float)v, 0, 127);
    }

    void ofApp::windowResized(int w, int h) {
        video_.windowResized(w, h);
    }
}
