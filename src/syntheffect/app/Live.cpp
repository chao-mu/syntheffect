#include "syntheffect/app/Live.h"

#include "GLFW/glfw3.h"

#include "ofMain.h"

#include "ofxTimeMeasurements.h"

#include "syntheffect/midi/MidiMessage.h"
#include "syntheffect/patch/PatchBuilder.h"

#define DISPLAY_KEYS {CHANNEL_OUT}
#define FPS 30


void setup() {
    TIME_SAMPLE_SET_FRAMERATE(FPS);
}

namespace syntheffect {
    namespace app {
            Live::Live(std::shared_ptr<LiveSettings> settings)
                : ofBaseApp(),
                renderer_(std::make_shared<Renderer>(settings->patch_path, settings->drawables)),
                joystick_(GLFW_JOYSTICK_1) {
            beat_ = std::make_shared<ofxBeat>();
            settings_ = settings;
        }

        void Live::setup() {
            ofSetFrameRate(FPS);

            ofSetBackgroundColor(0, 0, 0);
            ofSetBackgroundAuto(true);

            if (joystick_.isPresent()) {
                ofLogNotice() << "Joystick connected: " << joystick_.getName();
            }

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

            renderer_->setup();

            if (settings_->out_path != "") {
                recording_ = true;
                recorder_.setup(settings_->out_path, settings_->recording_width, settings_->recording_height);
                recorder_.startThread();
            } else {
                recording_ = false;
            }

            display_.load(renderer_->getWidth(), renderer_->getHeight(), ofGetWindowWidth(), ofGetWindowHeight());
        }

        void Live::exit() {
            recorder_.waitForThread();
        }

        void Live::audioIn(ofSoundBuffer& buf) {
            float *input = &buf.getBuffer()[0];
            size_t buffer_size = buf.getNumFrames();
            size_t channels = buf.getNumChannels();

            beat_->audioReceived(input, buffer_size, channels);
        }

        void Live::update() {
            float t = ofGetElapsedTimef();

            auto effect_params = std::make_shared<graphics::Params>();
            effect_params->float_params["time"] = ofGetElapsedTimef();

            beat_->update(ofGetElapsedTimeMillis());
            effect_params->float_params["kick"] = beat_->kick();
            effect_params->float_params["snare"] = beat_->snare();
            effect_params->float_params["hihat"] = beat_->hihat();

            joystick_.update();
            effect_params->set(joystick_.getParams());


            if (!renderer_->update(effect_params, t)) {
                ofExit();
            }
        }

        void Live::windowResized(int w, int h) {
            display_.windowResized(w, h);
        }


        void Live::draw() {
            if (!renderer_->isReady()) {
                return;
            }

            // Draw to display
            display_.draw(renderer_->channels, DISPLAY_KEYS);

            if (recording_) {
                recordFrame();
            }

            ofSetWindowTitle("fps: " + std::to_string(ofGetFrameRate()));
        }

        void Live::recordFrame() {
            recorder_.fboBegin();
            ofClear(0);
            renderer_->drawScaleCenter(recorder_.getWidth(), recorder_.getHeight());
            recorder_.fboEnd();
            recorder_.push();
        }

        void Live::screenshot() {
            ofFbo fbo;
            fbo.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);

            fbo.begin();
            display_.draw(renderer_->channels, DISPLAY_KEYS);
            fbo.end();

            ofPixels pixels;
            fbo.readToPixels(pixels);

            ofImage image;
            image.setFromPixels(pixels);
            image.save("out-" + ofGetTimestampString() + ".png");
        }

        void Live::keyPressed(int c) {
            if (c == 'p') {
                screenshot();
            } else if (c == 'q') {
                ofExit();
            }
        }
    }
}

#undef DISPLAY_KEYS
#undef SEEK_FRAMES
#undef FPS
