#include "syntheffect/rack/AudioAnalyzer.h"

namespace syntheffect {
    namespace rack {
        AudioAnalyzer::AudioAnalyzer(const std::string& id, const std::string& path, size_t buffer_size) : Module(id), path_(path), buffer_size_(buffer_size) {
        }

        void AudioAnalyzer::setup(int width, int height, int internal_format) {
            fft_ = std::shared_ptr<ofxFft>(ofxFft::create(buffer_size_, OF_FFT_WINDOW_HAMMING));

            player_.connectTo(*this);

            output_channels_["15khz"] = std::make_shared<Channel>(0);
            output_channels_["7-5khz"] = std::make_shared<Channel>(0);
            output_channels_["3khz"] = std::make_shared<Channel>(0);
            output_channels_["1-3khz"] = std::make_shared<Channel>(0);
            output_channels_["600hz"] = std::make_shared<Channel>(0);
            output_channels_["300hz"] = std::make_shared<Channel>(0);
            output_channels_["120hz"] = std::make_shared<Channel>(0);
            output_channels_["60hz"] = std::make_shared<Channel>(0);

            if (!player_.load(boost::filesystem::path(path_))) {
                throw std::runtime_error("error loading video with path " + path_);
            }
        }

        void AudioAnalyzer::start() {
            player_.play();
        }

        void AudioAnalyzer::update(float /* t */) {
            std::lock_guard<std::mutex> lock(buffer_mutex_);
            if (!buffer_allocated_) {
                return;
            }

            fft_->setSignal(buffer_.getBuffer());

            auto rate = buffer_.getSampleRate();
            output_channels_["15khz"]->value_ = fft_->getAmplitudeAtFrequency(15000, rate) * getInputConstant("15khzGain", 1);
            output_channels_["7-5khz"]->value_ = fft_->getAmplitudeAtFrequency(7500, rate) * getInputConstant("7-5khzGain", 1);
            output_channels_["3khz"]->value_ = fft_->getAmplitudeAtFrequency(3000, rate) * getInputConstant("3khzGain", 1);
            output_channels_["1-3khz"]->value_ = fft_->getAmplitudeAtFrequency(1300, rate) * getInputConstant("1-3khzGain", 1);
            output_channels_["600hz"]->value_ = fft_->getAmplitudeAtFrequency(600, rate) * getInputConstant("600hzGain", 1);
            output_channels_["300hz"]->value_ = fft_->getAmplitudeAtFrequency(300, rate) * getInputConstant("300hzGain", 1);
            output_channels_["120hz"]->value_ = fft_->getAmplitudeAtFrequency(120, rate) * getInputConstant("120hzGain", 1);
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
