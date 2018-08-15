#include "syntheffect/app/Live.h"

#include "GLFW/glfw3.h"

#include "ofMain.h"

#include "ofxTimeMeasurements.h"

#include "syntheffect/settings/ParamSettings.h"
#include "syntheffect/settings/ProjectSettings.h"

#define FPS 30

void setup() {
    TIME_SAMPLE_SET_FRAMERATE(FPS);
}

namespace syntheffect {
    namespace app {
        Live::Live(settings::ProjectSettings settings) : ofBaseApp(),  asset_manager_(settings.width, settings.height) {
            beat_ = std::make_shared<ofxBeat>();

            settings_ = settings;
        }

        void Live::setup() {
            ofSetFrameRate(FPS);

            ofSetBackgroundColor(0, 0, 0);
            ofSetBackgroundAuto(true);

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
            // Asset manager
            asset_manager_.setup();
            asset_manager_.setAssets(settings_.asset_groups);

            // Pipeline manager
            pipeline_manager_.setPipelines(settings_.pipelines);

            // Joystick manager
            for (auto js : settings_.joysticks) {
                joystick_manager_.addJoystick(js);
            }

            if (settings_.out_path != "") {
                recording_ = true;
                recorder_.setup(settings_.out_path, settings_.width, settings_.height);
                recorder_.startThread();
            } else {
                recording_ = false;
            }
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

            param::Params params;
            params.set(settings::ParamSettings::floatValue("time", t));

            for (auto& param : settings_.params) {
                params.set(param);
            }

            joystick_manager_.update(t);
            joystick_manager_.copyTo(params);

            /*
            beat_->update(ofGetElapsedTimeMillis());
            effect_params->float_params["time"] = ofGetElapsedTimef();

            effect_params->float_params["kick"] = beat_->kick();
            effect_params->float_params["snare"] = beat_->snare();
            effect_params->float_params["hihat"] = beat_->hihat();
            */

            asset_manager_.update(t, params);
            if (asset_manager_.isReady()) {
                if (asset_manager_.isFinished()) {
                    ofExit();
                }

                out_ = pipeline_manager_.render(t, params, asset_manager_.getChannels());
            }
        }

        void Live::draw() {
            if (!out_) {
                return;
            }

            out_->drawScaleCenter(ofGetWindowWidth(), ofGetWindowHeight());

            ofSetWindowTitle("fps: " + std::to_string(ofGetFrameRate()));
        }

        void Live::recordFrame() {
            if (!out_) {
                return;
            }

            recorder_.fboBegin();
            ofClear(0);
            out_->drawScaleCenter(recorder_.getWidth(), recorder_.getHeight());
            recorder_.fboEnd();
            recorder_.push();
        }

        void Live::screenshot() {
            ofFbo fbo;
            fbo.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);

            fbo.begin();
            out_->drawScaleCenter(settings_.width, settings_.height);
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

#undef FPS
