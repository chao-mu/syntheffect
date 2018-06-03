#include "syntheffect/app/DrawLoop.h"

#include "ofGraphics.h"

#include "syntheffect/patch/PatchBuilder.h"
#include "syntheffect/video/util.h"

#define DEBUG_CHANNEL_ACCESS false

namespace syntheffect {
    namespace app {
        DrawLoop::DrawLoop() : video() {
            ready_ = false;
        }

        float DrawLoop::getHeight() {
            return video.getHeight();
        }

        float DrawLoop::getWidth() {
            return video.getWidth(); }

        float DrawLoop::getFPS() {
            return video.getTotalNumFrames() / video.getDuration();
        }

        void DrawLoop::setLoopState(ofLoopType state) {
            video.setLoopState(state);
        }

        void DrawLoop::play() {
            video.play();
        }

        void DrawLoop::stop() {
            video.stop();
        }

        bool DrawLoop::load(std::string patch_path, std::string video_path) {
            video.setUseTexture(true);

            if (!video.load(video_path)) {
                ofLogError() <<  "Failed to load video '" << video_path << "'";
                return false;
            }

            channels = std::make_shared<syntheffect::graphics::PingPongBufferMap>(video.getWidth(), video.getHeight(), GL_RGB);
            channels->allocate(CHANNEL_ONE);
            channels->allocate(CHANNEL_OUT);
            channels->allocate(CHANNEL_LAST_OUT);
            channels->allocate(CHANNEL_LAST_CHANNEL_ONE);

            auto builder = std::make_unique<syntheffect::patch::PatchBuilder>();
            patch = builder->build(patch_path, channels);
            if (!patch) {
                ofLogError() << "Failed to build patch '" << patch_path << "'";
                return false;
            }

            return true;
        }

        bool DrawLoop::isReady() {
            return ready_;
        }

        bool DrawLoop::update(std::shared_ptr<graphics::Params> params, float t) {
            video.update(params, t);

            if (video.getIsMovieDone()) {
                return false;
            }

            if (!video.isReady()) {
                ofLogNotice() << std::to_string(video.isPlaying());
                return true;
            }

            patch->setEffectParams(params);

            // Read the video frame
            channels->get(CHANNEL_ONE)->drawFrom(video);

            // Apply effects/write to channels
            patch->drawTo(channels, t);

            // Write out channel to last-out channel
            channels->get(CHANNEL_OUT)->drawTo(channels->get(CHANNEL_LAST_OUT));

            // Write channel1 to last-channel1
            channels->get(CHANNEL_ONE)->drawTo(channels->get(CHANNEL_LAST_CHANNEL_ONE));

            if (DEBUG_CHANNEL_ACCESS) {
                for (auto& kv : channels->getAccessHistory()) {
                    if (!kv.second) {
                        ofLogWarning() << "Unaccessed channel: " + kv.first;
                    }
                }
            }

            ready_ = true;

            return true;
        }

        void DrawLoop::seek(int relative_frames) {
            video::seek(video, relative_frames);
        }
    }
}

#undef DEBUG_CHANNEL_ACCESS
