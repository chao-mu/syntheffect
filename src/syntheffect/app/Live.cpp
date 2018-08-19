#include "syntheffect/app/Live.h"

#include "GLFW/glfw3.h"

#include "ofMain.h"

#include "ofxTimeMeasurements.h"

#include "syntheffect/settings/ParamSettings.h"
#include "syntheffect/settings/ProjectSettings.h"

#include "syntheffect/asset/Parser.h"
#include "syntheffect/input/Parser.h"

#define FPS 30

void setup() {
    TIME_SAMPLE_SET_FRAMERATE(FPS);
}

namespace syntheffect {
    namespace app {
        Live::Live(settings::ProjectSettings settings) : ofBaseApp(),  renderer_(settings.width, settings.height) {
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

            // Input manager
            input::Parser::addInputs(input_manager_, settings_.inputs_path);
            ofAddListener(input_manager_.asset_trigger_events, this, &Live::assetGroupTriggered);
            ofAddListener(input_manager_.param_trigger_events, this, &Live::paramSetTriggered);

            // Asset manager
            asset_manager_.setAssets(asset::Parser::parseAssets(settings_.assets_path));

            // Renderer
            renderer_.setup();
            renderer_.setPipelines(settings_.pipelines);

            if (settings_.out_path != "") {
                recording_ = true;
                recorder_.setup(settings_.out_path, settings_.width, settings_.height);
                recorder_.startThread();
            } else {
                recording_ = false;
            }
        }

        void Live::paramSetTriggered(const settings::ParamSettings& p) {
            params_.set(p);
        }

        void Live::assetGroupTriggered(std::string& name) {
            asset_manager_.triggerAssetGroup(name);
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

            params_.set(settings::ParamSettings::floatValue("time", t));

            for (auto& param : settings_.params) {
                params_.set(param);
            }

            input_manager_.update(t);

            /*
            beat_->update(ofGetElapsedTimeMillis());
            effect_params->float_params["time"] = ofGetElapsedTimef();

            effect_params->float_params["kick"] = beat_->kick();
            effect_params->float_params["snare"] = beat_->snare();
            effect_params->float_params["hihat"] = beat_->hihat();
            */

            asset_manager_.update(t);
            if (asset_manager_.isReady()) {
                if (asset_manager_.isFinished()) {
                    ofExit();
                }

                renderer_.update(params_, asset_manager_.getActiveAssets());
            }
        }

        void Live::draw() {
            renderer_.draw(ofGetWindowWidth(), ofGetWindowHeight());

            ofSetWindowTitle("fps: " + std::to_string(ofGetFrameRate()));
        }

        void Live::recordFrame() {
            /*
            if (!out_) {
                return;
            }

            recorder_.fboBegin();
            ofClear(0);
            out_->drawScaleCenter(recorder_.getWidth(), recorder_.getHeight());
            recorder_.fboEnd();
            recorder_.push();
            */
        }

        void Live::keyPressed(int c) {
            if (c == 'p') {
                renderer_.saveImage("out-" + ofGetTimestampString() + ".png");
            } else if (c == 'q') {
                ofExit();
            }
        }
    }
}

#undef FPS
