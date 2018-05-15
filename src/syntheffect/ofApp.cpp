#include "syntheffect/ofApp.h"

#include "syntheffect/midi/MidiMessage.h"
#include "syntheffect/patch/PatchBuilder.h"

#define CHANNEL_ONE "channel1"
#define CHANNEL_OUT "out"
#define CHANNEL_LAST_OUT "last-out"
#define CHANNEL_LAST_CHANNEL_ONE "last-channel1"

#define DEBUG_CHANNEL_ACCESS true

// TODO: Move these into an external configuration
#define RECORDING false
#define DISPLAY_KEYS {CHANNEL_OUT}

#define FPS 30

// How many frames to seek forward/back in the video
#define SEEK_FRAMES 60

namespace syntheffect {
    ofApp::ofApp(std::shared_ptr<RtMidiIn> midi_in, std::string playlist_path) 
            : ofBaseApp(),
            midi_in_(move(midi_in)),
            effect_params_(std::make_shared<graphics::Params>()) {
        playlist_path_ = playlist_path;
        beat_ = std::make_shared<ofxBeat>();
    }

    void ofApp::setup() {
        is_recording_ = RECORDING;

        ofSetFrameRate(FPS);

        playlist_.load(playlist_path_);

        ofSetBackgroundAuto(true);
        ofSetFullscreen(true);

        #ifdef __APPLE__
            CGDisplayHideCursor(0);
        #else
            ofHideCursor();
        #endif

        nextVideo();
    
        int sample_rate = 44100;
        int channels = 2;

        std::vector<ofSoundDevice> sound_devices = sound_stream_.getMatchingDevices("ma++ ingalls for Cycling '74: Soundflower", 2, 2);
        if (sound_devices.size() < 1) {
            ofLogError() << "Soundflower device not found, sound features not enabled";
        } else {
            ofSoundDevice device = sound_devices[0];
            ofSoundStreamSettings sound_settings;
            sound_settings.setInDevice(device);
            sound_settings.setOutDevice(device);
            sound_settings.setInListener(this);
            sound_settings.numInputChannels = device.inputChannels;
            sound_settings.numOutputChannels = device.outputChannels;
            sound_settings.sampleRate = sample_rate;
            sound_settings.bufferSize = beat_->getBufferSize();

            ofSoundStreamSetup(sound_settings);
        }

        if (is_recording_) {
            vid_recorder_.setVideoCodec("mpeg4");
            vid_recorder_.setVideoBitrate("5000k");
            vid_recorder_.setAudioCodec("mp3");
            vid_recorder_.setAudioBitrate("192k");

            vid_recorder_.setup(
                "vid-out-" + ofGetTimestampString() + ".mp4",
                video_->getWidth(),
                video_->getHeight(),
                FPS//,
                //sample_rate,
                //channels
            );

            vid_recorder_.start();
        }
    }

    void ofApp::nextVideo() {
        video_ = playlist_.next();
        float w = video_->getWidth();
        float h = video_->getHeight();

        display_ = graphics::Display();
        display_.load(w, h, ofGetWidth(), ofGetHeight());

        channels_ = std::make_shared<graphics::PingPongBufferMap>(w, h, GL_RGB);

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
    void ofApp::audioIn(ofSoundBuffer& buf) {
        float *input = &(*buf.getBuffer().begin());
        size_t buffer_size = buf.size();
        size_t channels = buf.getNumChannels();  

        /*
        if(is_recording_) {
            vid_recorder_.addAudioSamples(input, buffer_size, channels);
        }
        */

        beat_->audioReceived(input, buffer_size, channels);
    }

    void ofApp::update() {
        if (!video_->update()) {
            stopRecording();
            nextVideo();
            video_->update();
        }

        if (!video_->isAllocated()) {
            return;
        }

        beat_->update(ofGetElapsedTimeMillis());

        effect_params_->float_params["kick"] = beat_->kick();
        effect_params_->float_params["snare"] = beat_->snare();
        effect_params_->float_params["hihat"] = beat_->hihat();

        patch_->setEffectParams(effect_params_);

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
        if (!channels_->isAccessed()) {
            return;
        }

        // Draw to display
        display_.draw(channels_, DISPLAY_KEYS);

        // Draw to recording
        if (is_recording_) {
            ofPixels pixels;
            channels_->get(CHANNEL_OUT)->drawable()->readToPixels(pixels);
            vid_recorder_.addFrame(pixels);
        }

        ofSetWindowTitle("fps: " + std::to_string(ofGetFrameRate()));
    }

    void ofApp::stopRecording() {
        vid_recorder_.close();
        is_recording_ = false;
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
            vid_recorder_.close();
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
