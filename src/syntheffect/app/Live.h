#pragma once

#include <vector>
#include <memory>

#include "ofMain.h"

#include "RtMidi.h"

#include "ofxVideoRecorder.h"
#include "ofxBeat.h"

#include "syntheffect/patch/Patch.h"
#include "syntheffect/graphics/Display.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/graphics/Drawable.h"
#include "syntheffect/app/Renderer.h"
#include "syntheffect/app/RecordingThread.h"
#include "syntheffect/app/LiveSettings.h"
#include "syntheffect/controller/XBoxController.h"

namespace syntheffect {
    namespace app {
        class Live : public ofBaseApp {
            public:
                Live(shared_ptr<LiveSettings> settings);
                void setup();
                void update();
                void draw();
                void audioIn(ofSoundBuffer& buf);
                void exit();

            protected:
                void windowResized(int w, int h);
                void keyPressed(int c);
                void screenshot();

            private:
                ofSoundStream sound_stream_;
                std::shared_ptr<ofxBeat> beat_;
                std::shared_ptr<RtMidiIn> midi_in_;
                graphics::Display display_;
                std::shared_ptr<app::Renderer> renderer_;

                void setupDrawSize();
                void recordFrame();

                bool should_draw_;
                bool recording_;

                std::shared_ptr<LiveSettings> settings_;
                RecordingThread recorder_;

                controller::XBoxController joystick_;
        };
    }
}
