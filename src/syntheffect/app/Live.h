#pragma once

#include <vector>
#include <memory>

#include "ofMain.h"

#include "ofxBeat.h"

#include "syntheffect/render/Renderer.h"
#include "syntheffect/asset/AssetManager.h"
#include "syntheffect/param/Param.h"
#include "syntheffect/input/InputManager.h"
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
                void exit();
                void audioIn(ofSoundBuffer& buf);

            protected:
                void keyPressed(int c);

                void assetGroupTriggered(std::string& name);
                void paramSetTriggered(const param::Param& p);

            private:
                void recordFrame();

                asset::AssetManager asset_manager_;
                render::Renderer renderer_;
                input::InputManager input_manager_;

                settings::ProjectSettings settings_;
                std::shared_ptr<ofxBeat> beat_;

                ofSoundStream sound_stream_;
                RecordingThread recorder_;

                bool recording_;

                param::Params params_;
        };
    }
}
