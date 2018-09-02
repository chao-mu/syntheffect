#ifndef SYNTHEFFECT_APP_LIVE_H
#define SYNTHEFFECT_APP_LIVE_H

#pragma once

#include <vector>
#include <memory>

#include "ofBaseApp.h"

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
                explicit Live(settings::ProjectSettings settings);

            protected:
                void setup() override;
                void update() override;
                void draw() override;
                void exit() override;
                void keyPressed(int c) override;
                void audioIn(ofSoundBuffer& buf) override;

                void handleControlState(const input::ControlState& state);

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

#endif
