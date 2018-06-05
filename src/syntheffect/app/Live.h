#pragma once

#include <vector>
#include <memory>

#include "ofMain.h"

#include "RtMidi.h"

#include "ofxVideoRecorder.h"
#include "ofxBeat.h"

#include "syntheffect/patch/Patch.h"
#include "syntheffect/midi/CmdMicro.h"
#include "syntheffect/graphics/Display.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/graphics/Drawable.h"
#include "syntheffect/app/Renderer.h"

namespace syntheffect {
    namespace app {
        class Live : public ofBaseApp, midi::CmdMicro {
            public:
                Live(std::string patch_path, std::vector<std::shared_ptr<graphics::Drawable>> drawables, std::string out_path);
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
                std::shared_ptr<app::Renderer> draw_loop_;
                ofxVideoRecorder recorder_;

                std::string patch_path_;
                std::string out_path_;

                void setupDrawSize();

                bool should_draw_;
                bool recording_;
        };
    }
}
