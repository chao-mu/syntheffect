#include "syntheffect/app/DrawLoop.h"

#include "ofGraphics.h"

#include "syntheffect/patch/PatchBuilder.h"
#include "syntheffect/video/util.h"

#define DEBUG_CHANNEL_ACCESS false

namespace syntheffect {
    namespace app {
        DrawLoop::DrawLoop()
        {
            drawable_ = false;
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

        bool DrawLoop::isDrawable() {
            return drawable_;
        }

        bool DrawLoop::update(std::shared_ptr<graphics::Params> effect_params, float t) {
            video.update();
            
            if (video.getIsMovieDone()) {
                return false;
            }
            
            if (!video.isInitialized()) {
                ofLogNotice() << std::to_string(video.isPlaying());
                return true;
            }

            patch->setEffectParams(effect_params);

            // Read the video frame
            std::shared_ptr<graphics::PingPongBuffer> channel_one = channels->get(CHANNEL_ONE);
            channel_one->begin();
            ofClear(255, 255, 255, 0);
            video.draw(0, 0);
            channel_one->end();

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

            drawable_ = true;

            return true;
        }

        void DrawLoop::seek(int relative_frames) {
            video::seek(video, relative_frames);
        }
    }
}

#undef DEBUG_CHANNEL_ACCESS
