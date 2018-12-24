#ifndef SYNTHEFFECT_APP_LIVE_H
#define SYNTHEFFECT_APP_LIVE_H

#pragma once

#include <vector>
#include <memory>

#include "ofBaseApp.h"
#include "ofSoundStream.h"

#include "syntheffect/rack/Rack.h"
#include "syntheffect/app/RecordingThread.h"
#include "syntheffect/graphics/DrawInfo.h"

namespace syntheffect {
    namespace app {
        class Live : public ofBaseApp {
            public:
                Live(const std::string& rack_path, const std::string& workspace_dir, const std::string& out_path, int fps);

            protected:
                void setup() override;
                void update() override;
                void draw() override;
                void exit() override;
                void keyPressed(int c) override;

                void screenshot();

            private:
                void recordFrame();

                void reload();

                const std::string workspace_dir_;
                const std::string out_path_;
                const std::string rack_path_;

                rack::Rack rack_;

                ofSoundStream sound_stream_;
                RecordingThread recorder_;

                bool recording_;
                int fps_;
        };
    }
}

#endif
