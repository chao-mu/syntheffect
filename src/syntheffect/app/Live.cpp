#include "syntheffect/app/Live.h"

#include "ofGraphics.h"

#include "ofxTimeMeasurements.h"

namespace syntheffect {
    namespace app {
        Live::Live(const std::string& rack_path, const std::string& workspace_dir, const std::string& out_path, int fps) :
            ofBaseApp(),
            workspace_dir_(workspace_dir),
            out_path_(out_path),
            rack_path_(rack_path),
            rack_(rack_path, workspace_dir),
            recording_(false),
            fps_(fps) {}

        void Live::reload() {
            rack_.stop();
            rack_ = rack::Rack(rack_path_, workspace_dir_);
            setup();
        }

        void Live::setup() {
            ofSetFrameRate(fps_);

            ofSetBackgroundColor(0, 0, 0);
            ofSetBackgroundAuto(true);

            ofHideCursor();
            
            ofSoundStreamSettings sound_settings;
            // this is here because jack was causing long delays or freezing at the start of video player,
            // with isNewFrame always returning false after the first frame. Even I uninstalled jack,
            // this app was still trying to use it and therefore failing.
#ifdef TARGET_LINUX
            ofLogNotice("Live", "Notice: Jack currently not supported on Linux");
            auto devices = sound_stream_.getDeviceList(ofSoundDevice::Api::PULSE);
            if (devices.size() > 0) {
                ofLogNotice("Live", "Using PulseAudio...");
                sound_settings.setOutDevice(devices[0]);
                sound_settings.setInDevice(devices[0]);
            }
#endif
            sound_settings.numInputChannels = 2;
            sound_settings.numOutputChannels = 2;
            sound_settings.sampleRate = 44100;
            sound_settings.bufferSize = 512;
            sound_settings.numBuffers = 4;
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
            rack_.stop();
        }

        void Live::update() {
            float t = ofGetElapsedTimef();

            bool all_ready = rack_.updateUnready(t);
            if (all_ready) {
                rack_.update(t);

                if (recording_) {
                    recordFrame();
                }
            }
        }

        void Live::draw() {
            ofTexture& tex = rack_.getTexture();
            graphics::DrawInfo draw_info = graphics::DrawInfo::scaleCenter(rack_.getWidth(), rack_.getHeight(), ofGetWindowWidth(), ofGetWindowHeight());

            tex.draw(
                draw_info.x_,
                draw_info.y_,
                draw_info.w_,
                draw_info.h_
            );

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
            const std::string path = ofFilePath::join(ofFilePath::join(out_dir, "rendered"), "out-" + ofGetTimestampString() + ".png");

            ofPixels pixels;
            rack_.getTexture().readToPixels(pixels);
            ofImage image;
            image.setFromPixels(pixels);
            image.save(path);
        }

        void Live::keyPressed(int c) {
            if (c == 'p') {
                screenshot();
            } else if (c == 'r') {
                reload();
            } else if (c == 'q') {
                ofExit();
            }
        }
    }
}

#undef FPS
