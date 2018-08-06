#pragma once

#include <vector>
#include <memory>

#include "ofMain.h"

#include "ofxBeat.h"

#include "syntheffect/graphics/Drawable.h"
#include "syntheffect/manager/Manager.h"
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
                void keyPressed(int c);
                void screenshot();

            private:
                std::shared_ptr<ofxBeat> beat_;
                std::shared_ptr<manager::Manager> manager_;
                std::shared_ptr<LiveSettings> settings_;
                std::shared_ptr<graphics::Drawable> out_;

                ofSoundStream sound_stream_;
                RecordingThread recorder_;
                controller::XBoxController joystick_;

                void setupDrawSize();
                void recordFrame();

                bool should_draw_;
                bool recording_;
        };
    }
}
