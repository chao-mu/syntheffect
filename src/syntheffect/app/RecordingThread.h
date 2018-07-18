#include <queue>

#include "ofPixels.h"
#include "ofThread.h"
#include "ofFbo.h"

namespace syntheffect {
    namespace app {
        class RecordingThread : public ofThread {
            public:
                void threadedFunction();
                void addFrame(ofPixels pixels);
                void setup(std::string filename, int width, int height);
                void fboBegin();
                void fboEnd();
                void push();
                int getWidth();
                int getHeight();

            private:
                int frame_count_ = 0;
                std::queue<ofPixels> pending;
                void work();
                std::string path_;
                ofFbo recording_buf_;

        };
    }
}
