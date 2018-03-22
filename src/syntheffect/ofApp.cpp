#include "syntheffect/ofApp.h"

#include "syntheffect/midi/MidiMessage.h"

namespace syntheffect {
    ofApp::ofApp(RtMidiIn* midi_in, std::string playlist_path) : ofBaseApp() {
        midi_in_ = midi_in;
        playlist_path_ = playlist_path;
    }

    void ofApp::setup(){
        playlist_.load(playlist_path_);

        video_ = playlist_.next();

        delay_filter_ = filter::Delay();
        filters_.append(delay_filter_);
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
        if (video_.isAllocated()) {
            delay_filter_.setLastTexture(video_.getLastTexture());
            video_.draw(filters_);
        }
    }

    void ofApp::onCmdMicroLeftLeftFaderSlide(unsigned char v) {
        delay_filter_.start();
        delay_filter_.setIntensity((float)v, 0, 127);
    }

    void ofApp::windowResized(int w, int h) {
        video_.windowResized(w, h);
    }
}
