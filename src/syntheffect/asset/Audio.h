#ifndef SYNTHEFFECT_ASSET_AUDIO
#define SYNTHEFFECT_ASSET_AUDIO

#include "ofxSoundPlayerObject.h"

#include "syntheffect/asset/Asset.h"

namespace syntheffect {
    namespace asset {
        class Audio : public Asset {
            public:
                Audio(const std::string& id, const std::string& path, float volume, bool loop);
                void setup() override;
                bool isReady() override;
                void restart() override;
                void play() override;
                void pause() override;
                bool isFinished() override;

                ofxSoundObject& getSoundObject();

            private:
                std::string path_;
                ofxSoundPlayerObject player_;
                float volume_;
                bool loop_;

                int sample_rate_ = 44100;
                int buffer_size_ = 512;
                int channels_ = 2;
        };
    }
}
#endif
