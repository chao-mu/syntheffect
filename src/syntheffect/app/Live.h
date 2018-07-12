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

#include "syntheffect/app/LiveSettings.h"

namespace syntheffect {
    namespace app {
        class Live : public ofBaseApp {
            public:
                Live(shared_ptr<LiveSettings> settings);
                void setup();
                void update();
                void draw();
                void audioIn(ofSoundBuffer& buf);
                void safeExit();
                void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);

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
                ofxVideoRecorder recorder_;

                void setupDrawSize();

                bool should_draw_;
                bool recording_;

                ofFbo recording_buf_;

                std::shared_ptr<LiveSettings> settings_;
        };
    }
}
