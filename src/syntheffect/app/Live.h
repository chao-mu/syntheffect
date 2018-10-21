#ifndef SYNTHEFFECT_APP_LIVE_H
#define SYNTHEFFECT_APP_LIVE_H

#pragma once

#include <vector>
#include <memory>

#include "ofBaseApp.h"
#include "ofSoundStream.h"

#include "syntheffect/rack/Rack.h"
#include "syntheffect/param/Param.h"
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
                //void audioIn(ofSoundBuffer& buf) override;

                void screenshot();

            private:
                void recordFrame();

                rack::Rack rack_;

                settings::ProjectSettings settings_;

                ofSoundStream sound_stream_;
                RecordingThread recorder_;

                bool recording_;
        };
    }
}

#endif
