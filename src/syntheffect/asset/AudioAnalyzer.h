#ifndef SYNTHEFFECT_ASSET_AUDIOANALYZER
#define SYNTHEFFECT_ASSET_AUDIOANALYZER

#include <mutex>

#include "ofxSoundObjects.h"
#include "ofSoundBuffer.h"

#include "syntheffect/param/Params.h"

namespace syntheffect {
    namespace asset {
        class AudioAnalyzer : public ofxSoundObject {
            public:
                void setup(size_t num_samples, size_t num_channels);
                void process(ofSoundBuffer &input, ofSoundBuffer &output);
                void copyParamsTo(param::Params& p);

            private:
                float rms_ = 0;
                std::mutex buffer_mutex_;
        };
    }
}
#endif
