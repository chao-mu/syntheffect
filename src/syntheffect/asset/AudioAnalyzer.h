#ifndef SYNTHEFFECT_ASSET_AUDIOANALYZER
#define SYNTHEFFECT_ASSET_AUDIOANALYZER

#include <mutex>
#include <memory>

#include "ofxSoundObjects.h"
#include "ofSoundBuffer.h"
#include "ofxFft.h"

#include "syntheffect/param/Params.h"

namespace syntheffect {
    namespace asset {
        class AudioAnalyzer : public ofxSoundObject {
            public:
                void setup(size_t buffer_size);
                void process(ofSoundBuffer &input, ofSoundBuffer &output);
                void copyParamsTo(param::Params& p);

            private:
                bool buffer_allocated_ = false;
                std::mutex buffer_mutex_;
                ofSoundBuffer buffer_;
                std::shared_ptr<ofxFft> fft_;
        };
    }
}
#endif
