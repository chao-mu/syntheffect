#include "syntheffect/app/Live.h"

#include "GLFW/glfw3.h"

#include "ofGraphics.h"

#include "ofxTimeMeasurements.h"

#include "syntheffect/param/Param.h"
#include "syntheffect/settings/ProjectSettings.h"
#include "syntheffect/input/Parser.h"
#include "syntheffect/asset/Drawable.h"

#define FPS 30

void setup() {
    //TIME_SAMPLE_SET_FRAMERATE(FPS);
}

namespace syntheffect {
    namespace app {
        Live::Live(const settings::ProjectSettings settings) :
            ofBaseApp(),
            rack_(settings.rack_path),
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

            rack_.setup(settings_.width, settings_.height);
            rack_.start();

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
        }

        void Live::exit() {
            recorder_.waitForThread();
        }

        void Live::update() {
            float t = ofGetElapsedTimef();

            params_.set(param::Param::floatValue("time", t));

            TS_START_NIF("rack_.updateUnready");
            bool all_ready = rack_.updateUnready(t);
            TS_STOP_NIF("rack_.updateUnready");
            if (all_ready) {
                TS_START_NIF("rack_.update");
                rack_.update(t);
                TS_STOP_NIF("rack_.update");

                if (recording_) {
                    recordFrame();
                }
            }
        }

        void Live::draw() {
            TS_START("rack_.draw");
            ofTexture& tex = rack_.getTexture();
            asset::Drawable::drawScaleCenter(tex.getWidth(), tex.getHeight(), ofGetWindowWidth(), ofGetWindowHeight(),
                    [tex](float x, float y, float w, float h) { tex.draw(x, y, w, h); });
            TS_STOP("rack_.draw");

            ofSetWindowTitle("fps: " + std::to_string(ofGetFrameRate()));
        }

        void Live::recordFrame() {
            ofTexture& tex = rack_.getTexture();
            recorder_.fboBegin();
            ofClear(0);
            tex.draw(0, 0, recorder_.getWidth(), recorder_.getHeight());
            recorder_.fboEnd();
            recorder_.push();
        }

        void Live::screenshot() {
            ofDirectory::createDirectory(settings_.out_dir);
            const std::string path = ofFilePath::join(settings_.out_dir, "out-" + ofGetTimestampString() + ".png");

            ofPixels pixels;
            rack_.getTexture().readToPixels(pixels);
            ofImage image;
            image.setFromPixels(pixels);
            image.save(path);
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
