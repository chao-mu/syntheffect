#ifndef SYNTHEFFECT_APP_LIVE_H
#define SYNTHEFFECT_APP_LIVE_H

#pragma once

#include <vector>
#include <memory>

#include "ofBaseApp.h"
#include "ofSoundStream.h"

#include "syntheffect/rack/Rack.h"
#include "syntheffect/app/RecordingThread.h"

namespace syntheffect {
    namespace app {
        class Live : public ofBaseApp {
            public:
                Live(const std::string& rack_path, const std::string& modules_dir, const std::string& out_path);

            protected:
                void setup() override;
                void update() override;
                void draw() override;
                void exit() override;
                void keyPressed(int c) override;

                void screenshot();

            private:
                void recordFrame();

                const std::string out_path_;
                const std::string rack_path_;

                rack::Rack rack_;

                ofSoundStream sound_stream_;
                RecordingThread recorder_;

                bool recording_;
        };
    }
}

#endif
