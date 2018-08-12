#pragma once

#include <vector>
#include <memory>

#include "ofMain.h"

#include "ofxBeat.h"

#include "syntheffect/manager/PipelineManager.h"
#include "syntheffect/manager/JoystickManager.h"
#include "syntheffect/graphics/Drawable.h"
#include "syntheffect/app/RecordingThread.h"
#include "syntheffect/settings/ProjectSettings.h"

namespace syntheffect {
    namespace app {
        class Live : public ofBaseApp {
            public:
                Live(settings::ProjectSettings settings);
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
                manager::PipelineManager pipeline_manager_;
                manager::JoystickManager joystick_manager_;
                std::shared_ptr<graphics::Drawable> out_;
                settings::ProjectSettings settings_;

                ofSoundStream sound_stream_;
                RecordingThread recorder_;

                void setupDrawSize();
                void recordFrame();

                bool should_draw_;
                bool recording_;
        };
    }
}
