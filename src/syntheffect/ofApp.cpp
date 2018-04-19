#include "syntheffect/ofApp.h"

#include "syntheffect/midi/MidiMessage.h"
#include "syntheffect/patch/PatchBuilder.h"

#define CHANNEL_ONE "channel1"
#define CHANNEL_OUT "out"
#define CHANNEL_LAST_OUT "last-out"

#define DEBUG_CHANNEL_ACCESS true

// How many frames to seek forward/back in the video
#define SEEK_FRAMES 20

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
        float w = video_->getWidth();
        float h = video_->getHeight();

        display_ = graphics::Display();
        display_.load(w, h, ofGetWidth(), ofGetHeight());

        channels_ = make_shared<graphics::PingPongBufferMap>(w, h);

        channels_->allocate(CHANNEL_ONE);
        channels_->allocate(CHANNEL_OUT);
        channels_->allocate(CHANNEL_LAST_OUT);

        unique_ptr<patch::PatchBuilder> builder = make_unique<patch::PatchBuilder>();

        patch_ = builder->build("patches/default.xml", channels_);
        if (!patch_) {
            throw runtime_error("Unable to build patch!");
        }
    }

    void ofApp::update() {
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
        if (!video_->update()) {
            nextVideo();
            video_->update();
        }

        if (!video_->isAllocated()) {
            return;
        }

        // Read the video frame
        video_->draw(channels_->get(CHANNEL_ONE));

        // Apply effects/write to channels
        patch_->draw(channels_, ofGetElapsedTimef());

        // Draw to display
        display_.draw(channels_->get(CHANNEL_OUT));

        // Write out channel to last-out channel
        shared_ptr<graphics::PingPongBuffer> last_out = channels_->get(CHANNEL_LAST_OUT);
        last_out->begin();
        channels_->get(CHANNEL_OUT)->drawable()->draw(0, 0);
        last_out->end();

        if (DEBUG_CHANNEL_ACCESS) {
            for (auto& kv : channels_->getAccessHistory()) {
                if (!kv.second) {
                    ofLogWarning() << "Unaccessed channel: " + kv.first;
                }
            }
        }

        ofSetWindowTitle("fps: " + std::to_string(ofGetFrameRate()));
    }

    void ofApp::keyPressed(int c) {
        if (c == 'p') {
            ofPixels pixels;
            channels_->get(CHANNEL_LAST_OUT)->drawable()->readToPixels(pixels);

            ofImage image;
            image.setFromPixels(pixels);
            image.save("out-" + ofGetTimestampString() + ".png");
        } else if (c == OF_KEY_LEFT) {
            video_->seek(-SEEK_FRAMES);
        } else if (c == OF_KEY_RIGHT) {
            video_->seek(SEEK_FRAMES);
        }
    }

    void ofApp::onCmdMicroLeftLeftFaderSlide(unsigned char v) {
        // delay_filter_->start();
        // delay_filter_->setIntensity((float)v, 0, 127);
    }
}

#undef CHANNEL_ONE
#undef CHANNEL_OUT
#undef CHANNEL_LAST_OUT
#undef DEBUG_CHANNEL_ACCESS
#undef SEEK_FRAMES