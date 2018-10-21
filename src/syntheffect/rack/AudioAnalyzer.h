#ifndef SYNTHEFFECT_RACK_AUDIOANALYZER
#define SYNTHEFFECT_RACK_AUDIOANALYZER

#include <mutex>
#include <memory>

#include "boost/filesystem.hpp"

#include "ofxSoundObjects.h"
#include "ofSoundBuffer.h"
#include "ofxFft.h"

#include "syntheffect/rack/Module.h"

namespace syntheffect {
    namespace rack {
        class AudioAnalyzer : public Module, public ofxSoundObject {
            public:
                AudioAnalyzer(const std::string& id, const std::string& path, size_t buffer_size);
                void process(ofSoundBuffer &input, ofSoundBuffer &output) override;
                void update(float t) override;
                void setup(int width, int height, int internal_format) override;
                void start() override;

            private:
                ofxSoundPlayerObject player_;
                bool buffer_allocated_ = false;
                std::mutex buffer_mutex_;
                ofSoundBuffer buffer_;
                std::shared_ptr<ofxFft> fft_;
                const std::string path_;
                size_t buffer_size_;
        };
    }
}
#endif
