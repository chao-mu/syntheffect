#include "syntheffect/ofApp.h"

#include "syntheffect/midi/MidiMessage.h"
#include "syntheffect/patch/PatchBuilder.h"

#define CHANNEL_ONE "channel1"
#define CHANNEL_OUT "out"
#define CHANNEL_LAST_OUT "last-out"
#define CHANNEL_LAST_CHANNEL_ONE "last-channel1"

#define DEBUG_CHANNEL_ACCESS true

// How many frames to seek forward/back in the video
#define SEEK_FRAMES 20

const vector<std::string> DISPLAY_KEYS = {"outline", CHANNEL_OUT};

namespace syntheffect {
    ofApp::ofApp(shared_ptr<RtMidiIn> midi_in, std::string playlist_path) 
            : ofBaseApp(),
            midi_in_(move(midi_in)) {
        playlist_path_ = playlist_path;
    }

    void ofApp::setup() {
        playlist_.load(playlist_path_);

        ofSetBackgroundAuto(true);
        ofSetFullscreen(true);

        #ifdef __APPLE__
            CGDisplayHideCursor(0);
        #else
            ofHideCursor();
        #endif

        nextVideo();
    }

    void ofApp::nextVideo() {
        video_ = playlist_.next();
        float w = video_->getWidth();
        float h = video_->getHeight();

        display_ = graphics::Display();
        display_.load(w, h, ofGetWidth(), ofGetHeight());

        channels_ = make_shared<graphics::PingPongBufferMap>(w, h, GL_RGB);

        channels_->allocate(CHANNEL_ONE);
        channels_->allocate(CHANNEL_OUT);
        channels_->allocate(CHANNEL_LAST_OUT);
        channels_->allocate(CHANNEL_LAST_CHANNEL_ONE);

        unique_ptr<patch::PatchBuilder> builder = make_unique<patch::PatchBuilder>();

        patch_ = builder->build("patches/default.xml", channels_);
        if (!patch_) {
            throw runtime_error("Unable to build patch!");
        }
    }

    void ofApp::update() {
        if (!video_->update()) {
            nextVideo();
            video_->update();
        }

        if (!video_->isAllocated()) {
            return;
        }

        if (!video_->isFrameNew()) {
            return;
        }

        std::vector<unsigned char> raw_message;
        while (midi_in_->getMessage(&raw_message) > 0) {
            midi::MidiMessage msg(raw_message);
            handleCmdMicroEvent(msg);
        }

        // Read the video frame
        video_->drawTo(channels_->get(CHANNEL_ONE));

        // Apply effects/write to channels
        patch_->drawTo(channels_, ofGetElapsedTimef());

        // Write out channel to last-out channel
        channels_->get(CHANNEL_OUT)->drawTo(channels_->get(CHANNEL_LAST_OUT));

        // Write channel1 to last-channel1
        channels_->get(CHANNEL_ONE)->drawTo(channels_->get(CHANNEL_LAST_CHANNEL_ONE));

        if (DEBUG_CHANNEL_ACCESS) {
            for (auto& kv : channels_->getAccessHistory()) {
                if (!kv.second) {
                    ofLogWarning() << "Unaccessed channel: " + kv.first;
                }
            }
        }
    }

    void ofApp::windowResized(int w, int h) {
        display_.windowResized(w, h);
    }

    void ofApp::draw() {
        ofSetWindowTitle("fps: " + std::to_string(ofGetFrameRate()));

        // Draw to display
        display_.draw(channels_, DISPLAY_KEYS);
    }

    void ofApp::screenshot() {
        ofFbo fbo;
        fbo.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);

        fbo.begin();
        display_.draw(channels_, DISPLAY_KEYS);
        fbo.end();

        ofPixels pixels;
        fbo.readToPixels(pixels);

        ofImage image;
        image.setFromPixels(pixels);
        image.save("out-" + ofGetTimestampString() + ".png");
    }

    void ofApp::keyPressed(int c) {
        if (c == 'p') {
            screenshot();
        } else if (c == 'q') {
            ofExit();
        } else if (c == OF_KEY_LEFT) {
            video_->seek(-SEEK_FRAMES);
        } else if (c == OF_KEY_RIGHT) {
            video_->seek(SEEK_FRAMES);
        }
    }
}

#undef CHANNEL_ONE
#undef CHANNEL_OUT
#undef CHANNEL_LAST_OUT
#undef CHANNEL_LAST_CHANNEL_ONE
#undef DEBUG_CHANNEL_ACCESS
#undef SEEK_FRAMES