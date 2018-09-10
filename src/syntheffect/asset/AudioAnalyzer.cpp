#include "syntheffect/asset/AudioAnalyzer.h"

#include "ofLog.h"

namespace syntheffect {
    namespace asset {
        void AudioAnalyzer::setup(size_t buffer_size) {
            fft_ = std::shared_ptr<ofxFft>(ofxFft::create(buffer_size, OF_FFT_WINDOW_HAMMING));
        }

        void AudioAnalyzer::copyParamsTo(param::Params& p) {
            static std::vector<float> bark_scale = {50, 150, 250, 350, 450, 570, 700, 840, 1000, 1170, 1370, 1600, 1850, 2150, 2500, 2900, 3400, 4000, 4800, 5800, 7000, 8500, 10500, 13500};

            buffer_mutex_.lock();
            fft_->setSignal(buffer_.getBuffer());

            float* real = fft_->getReal();
            float* imag = fft_->getImaginary();
            float* amplitude = fft_->getAmplitude();
            size_t bin_size = fft_->getBinSize();

            float low = 0;
            float mid = 0;
            float high = 0;
            for (const auto& bark_hz : bark_scale) {
                std::string line;
                float amp = fft_->getAmplitudeAtFrequency(bark_hz, buffer_.getSampleRate());
                for (size_t j=0; j < amp * 100; j++) {
                    line += "-";
                }

                if (bark_hz <= 250) {
                    low +=  amp;
                } else if (bark_hz <= 570) {
                    mid +=  amp;
                } else {
                    high += amp;
                }
            }

            p.set(param::Param::floatValue("audio-rms", buffer_.getRMSAmplitude()));
            p.set(param::Param::floatValue("audio-low", low));
            p.set(param::Param::floatValue("audio-mid", mid));
            p.set(param::Param::floatValue("audio-high", high));

            buffer_mutex_.unlock();
        }

        void AudioAnalyzer::process(ofSoundBuffer &input, ofSoundBuffer &output) {
            input.copyTo(output);
            // Better to skip if the lock is open than wait for it
            if (!buffer_mutex_.try_lock()) {
                return;
            }

            if (!buffer_allocated_ || input.getNumFrames() != buffer_.getNumFrames() || input.getNumChannels() != buffer_.getNumChannels()) {
                buffer_ = ofSoundBuffer();
                buffer_.setNumChannels(input.getNumChannels());
                buffer_.allocate(input.getNumFrames(), input.getNumChannels());
                buffer_allocated_ = true;
            }

            input.copyTo(buffer_);

            buffer_mutex_.unlock();
        }
    }
}
