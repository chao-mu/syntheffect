#include "syntheffect/app/Live.h"

#include "GLFW/glfw3.h"

#include "ofGraphics.h"

#include "ofxTimeMeasurements.h"

#include "syntheffect/param/Param.h"
#include "syntheffect/settings/ProjectSettings.h"

#include "syntheffect/asset/Parser.h"
#include "syntheffect/input/Parser.h"

#define FPS 30

void setup() {
    TIME_SAMPLE_SET_FRAMERATE(FPS);
}

namespace syntheffect {
    namespace app {
        Live::Live(const settings::ProjectSettings settings) :
            ofBaseApp(),
            renderer_(settings.width, settings.height),
            settings_(settings),
            recording_(false) {}

        void Live::setup() {
            ofSetFrameRate(FPS);

            ofSetBackgroundColor(0, 0, 0);
            ofSetBackgroundAuto(true);

            #ifdef __APPLE__
                CGDisplayHideCursor(0);
            #else
                ofHideCursor();
            #endif

            // Input manager
            input::Parser::addInputs(input_manager_, settings_.inputs_path);
            ofAddListener(input_manager_.state_events, this, &Live::handleControlState);

            ofSoundStreamSettings sound_settings;
            sound_settings.numInputChannels = 0;
            sound_settings.numOutputChannels = 2;
            sound_settings.sampleRate = 44100;
            sound_settings.bufferSize = 512;
            sound_settings.numBuffers = 1;
            sound_stream_.setup(sound_settings);
            sound_stream_.setOutput(asset_manager_.getSoundOutput());

            asset_manager_.setup(sound_settings.bufferSize);

            // Asset manager
            asset::Parser::addAssets(asset_manager_, settings_.assets_path);

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

        void Live::handleControlState(const input::ControlState& state) {
            if (state.pressed) {
                for (const auto& p : state.mapping.pressed_param) {
                    if (p.variable_value == "press_time") {
                        params_.set(p.withValue(state.pressed_time));
                    } else {
                        params_.set(p);
                    }
                }
            } else {
                for (const auto& p : state.mapping.unpressed_param) {
                    params_.set(p);
                }
            }

            if (state.first_press) {
                for (const auto& asset : state.mapping.first_press_activate_asset) {
                    asset_manager_.activateAsset(asset);
                }
                for (const auto& group : state.mapping.first_press_activate_group) {
                    asset_manager_.activateGroup(group);
                }
                for (const auto& stack : state.mapping.first_press_shuffle_active_stack) {
                    asset_manager_.shuffleActiveStack(stack);
                }
                for (const auto& stack : state.mapping.first_press_next_stack_asset) {
                    asset_manager_.nextStackAsset(stack);
                }
                for (const auto& stack : state.mapping.first_press_prev_stack_asset) {
                    asset_manager_.prevStackAsset(stack);
                }
            }
        }

        void Live::exit() {
            recorder_.waitForThread();
        }

        void Live::update() {
            float t = ofGetElapsedTimef();

            params_.set(param::Param::floatValue("time", t));

            input_manager_.update(t);

            asset_manager_.update(t);
            if (asset_manager_.isReady()) {
                if (asset_manager_.isFinished()) {
                    ofExit();
                }

                asset_manager_.copyParamsTo(params_);

                renderer_.update(params_, asset_manager_.getDrawables());

                if (recording_) {
                    recordFrame();
                }
            }
        }

        void Live::draw() {
            renderer_.draw(ofGetWindowWidth(), ofGetWindowHeight());

            ofSetWindowTitle("fps: " + std::to_string(ofGetFrameRate()));
        }

        void Live::recordFrame() {
            recorder_.fboBegin();
            ofClear(0);
            renderer_.draw(recorder_.getWidth(), recorder_.getHeight());
            recorder_.fboEnd();
            recorder_.push();
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
