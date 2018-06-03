#include "syntheffect/app/Live.h"

#include "RtMidi.h"

#include "ofMain.h"

#include "syntheffect/midi/MidiMessage.h"
#include "syntheffect/patch/PatchBuilder.h"

#define DISPLAY_KEYS {CHANNEL_OUT}
#define RECORDING_FPS 30

// How many frames to seek forward/back in the video
#define SEEK_FRAMES 60

namespace syntheffect {
    namespace app {
            Live::Live(std::string patch_path, std::string video_path, std::string out_path)
                : ofBaseApp(),
                draw_loop_(std::make_shared<DrawLoop>()) {
            beat_ = std::make_shared<ofxBeat>();
            patch_path_ = patch_path;
            video_path_ = video_path;
            out_path_ = out_path;
        }

        void Live::setup() {
            ofSetBackgroundAuto(true);
            ofSetFullscreen(false);

            #ifdef __APPLE__
                CGDisplayHideCursor(0);
            #else
                ofHideCursor();
            #endif

                /* Commening out old code that will only work on Mac, keeping in case of wanting  to be cross platform
            std::vector<ofSoundDevice> sound_devices = sound_stream_.getMatchingDevices("ma++ ingalls for Cycling '74: Soundflower", 2, 2);
            if (sound_devices.size() < 1) {
                ofLogWarning() << "Soundflower device not found, sound features not enabled";
            } else {
                ofSoundDevice device = sound_devices[0];
                ofSoundStreamSettings sound_settings;
                sound_settings.setInDevice(device);
                sound_settings.setOutDevice(device);
                sound_settings.setInListener(this);
                sound_settings.numInputChannels = device.inputChannels;
                sound_settings.numOutputChannels = device.outputChannels;
                sound_settings.sampleRate = 44100;;

                sound_settings.bufferSize = beat_->getBufferSize();

                ofSoundStreamSetup(sound_settings);
            }
            */

            if (!draw_loop_->load(patch_path_, video_path_)) {
                ofLogFatalError() << "Failed to load draw loop!";
                ofExit();
            }

            draw_loop_->play();


            if (out_path_ != "") {
                ofSetFrameRate(RECORDING_FPS);

                recorder_.setVideoCodec("libx265");
                recorder_.setVideoBitrate("8000k");

                recorder_.setup(
                    out_path_,
                    draw_loop_->getWidth(),
                    draw_loop_->getHeight(),
                    RECORDING_FPS
                );

                recorder_.start();

                ofAddListener(recorder_.outputFileCompleteEvent, this, &Live::recordingComplete);

                recording_ = true;
            } else {
                recording_ = false;
            }

            display_.load(draw_loop_->getWidth(), draw_loop_->getHeight(), ofGetWindowWidth(), ofGetWindowHeight());
        }

        void Live::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args) {
            recording_ = false;
        }

        void Live::audioIn(ofSoundBuffer& buf) {
            float *input = &buf.getBuffer()[0];
            size_t buffer_size = buf.getNumFrames();
            size_t channels = buf.getNumChannels();

            beat_->audioReceived(input, buffer_size, channels);
        }

        void Live::update() {
            beat_->update(ofGetElapsedTimeMillis());


            auto effect_params = std::make_shared<graphics::Params>();
            effect_params->float_params["kick"] = beat_->kick();
            effect_params->float_params["snare"] = beat_->snare();
            effect_params->float_params["hihat"] = beat_->hihat();

            if (!draw_loop_->update(effect_params, ofGetElapsedTimef())) {
                safeExit();
            }
        }

        void Live::windowResized(int w, int h) {
            display_.windowResized(w, h);
        }

        void Live::draw() {
            if (!draw_loop_->isReady()) {
                return;
            }

            // Draw to display
            display_.draw(draw_loop_->channels, DISPLAY_KEYS);

            if (recording_) {
                ofPixels pixels;
                draw_loop_->channels->get(CHANNEL_OUT)->drawable()->readToPixels(pixels);
                recorder_.addFrame(pixels);
            }

            ofSetWindowTitle("fps: " + std::to_string(ofGetFrameRate()));
        }

        void Live::screenshot() {
            ofFbo fbo;
            fbo.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);

            fbo.begin();
            display_.draw(draw_loop_->channels, DISPLAY_KEYS);
            fbo.end();

            ofPixels pixels;
            fbo.readToPixels(pixels);

            ofImage image;
            image.setFromPixels(pixels);
            image.save("out-" + ofGetTimestampString() + ".png");
        }

        void Live::safeExit() {
            draw_loop_->stop();

            if (recording_) {
                recorder_.close();
            }

            while (recording_) {
                usleep(10 * 1000);
            }
            ofExit();
        }

        void Live::keyPressed(int c) {
            if (c == 'p') {
                screenshot();
            } else if (c == 'q') {
                safeExit();
            } else if (c == OF_KEY_LEFT) {
                draw_loop_->seek(-SEEK_FRAMES);
            } else if (c == OF_KEY_RIGHT) {
                draw_loop_->seek(SEEK_FRAMES);
            }
        }
    }
}
#undef DISPLAY_KEYS
#undef SEEK_FRAMES
#undef RECORDING_FPS
