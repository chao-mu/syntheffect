#include "syntheffect/app/Live.h"

#include "GLFW/glfw3.h"

#include "ofGraphics.h"

#include "ofxTimeMeasurements.h"

#include "syntheffect/graphics/Util.h"

#define FPS 30

void setup() {
    //TIME_SAMPLE_SET_FRAMERATE(FPS);
}

namespace syntheffect {
    namespace app {
        Live::Live(const std::string& rack_path, const std::string& out_path) :
            ofBaseApp(),
            rack_(rack_path),
            out_path_(out_path),
            rack_path_(rack_path),
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

            ofSoundStreamSettings sound_settings;
            sound_settings.numInputChannels = 0;
            sound_settings.numOutputChannels = 2;
            sound_settings.sampleRate = 44100;
            sound_settings.bufferSize = 512;
            sound_settings.numBuffers = 1;
            sound_stream_.setup(sound_settings);
            sound_stream_.setOutput(rack_.getSoundOutput());

            rack_.setup(sound_settings.bufferSize);
            rack_.start();

            if (out_path_ != "") {
                recording_ = true;
                recorder_.setup(out_path_, rack_.getWidth(), rack_.getHeight());
                recorder_.startThread();
            } else {
                recording_ = false;
            }
        }

        void Live::exit() {
            recorder_.waitForThread();
        }

        void Live::update() {
            float t = ofGetElapsedTimef();

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
            graphics::Util::drawScaleCenter(tex.getWidth(), tex.getHeight(), ofGetWindowWidth(), ofGetWindowHeight(),
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
            const std::string out_dir = ofFilePath::getEnclosingDirectory(rack_path_);
            ofDirectory::createDirectory(out_dir);
            const std::string path = ofFilePath::join(out_dir, "out-" + ofGetTimestampString() + ".png");

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
