#pragma once

#include <string>

#include "ofxVideoRecorder.h"
#include "syntheffect/app/DrawLoop.h"

namespace syntheffect {
    namespace app {
        class Record : public ofBaseApp {
            public:
                Record(std::string patch_path, std::string video_path);
                void setup();
                void update();

            private:
                float t_;
                float t_increment_;
                ofxVideoRecorder vid_recorder_;
                std::shared_ptr<DrawLoop> draw_loop_;

                std::string video_path_;
                std::string patch_path_;
                int last_frame_;
        };
    }
}
