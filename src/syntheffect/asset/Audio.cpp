#include "syntheffect/asset/Audio.h"

#include "boost/filesystem.hpp"

#include "ofMath.h"
#include "ofLog.h"

namespace syntheffect {
    namespace asset {
        Audio::Audio(const std::string& id, const std::string& path, float volume, bool loop) :
            Asset(id),
            path_(path),
            volume_(volume),
            loop_(loop) {}

        void Audio::setup() {
            if (!player_.load(boost::filesystem::path(path_))) {
                throw std::runtime_error("Error loading video with path " + path_);
            }

            player_.setLoop(loop_);
        }

        bool Audio::isReady() {
            return player_.isLoaded();
        }

        void Audio::restart() {
            player_.setPosition(0);
        }

        void Audio::play() {
            player_.play();
            player_.setVolume(volume_);
        }

        void Audio::pause() {
            player_.setPaused(true);
        }

        bool Audio::isFinished() {
            return !loop_ && ofIsFloatEqual(player_.getPosition(), 1.0f);
        }

        ofxSoundObject& Audio::getSoundObject() {
            return player_;
        }
    }
}
