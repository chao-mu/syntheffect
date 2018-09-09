#include "syntheffect/asset/AudioAnalyzer.h"

namespace syntheffect {
    namespace asset {
        void AudioAnalyzer::copyParamsTo(param::Params& p) {
            buffer_mutex_.lock();
            p.set(param::Param::floatValue("audio-rms", rms_));
            buffer_mutex_.unlock();
        }

        void AudioAnalyzer::process(ofSoundBuffer &input, ofSoundBuffer &output) {
            input.copyTo(output);
            if (buffer_mutex_.try_lock()) {
                rms_ = output.getRMSAmplitude();
                buffer_mutex_.unlock();
            }
        }
    }
}
