#include "syntheffect/app/Renderer.h"

#include <string>
#include <stdexcept>

#include "ofGraphics.h"

#include "syntheffect/patch/PatchBuilder.h"
#include "syntheffect/video/util.h"

#define DEBUG_CHANNEL_ACCESS false

namespace syntheffect {
    namespace app {
        Renderer::Renderer(std::string patch_path, std::vector<std::shared_ptr<graphics::Drawable>> drawables) {
            ready_ = false;
            patch_path_ = patch_path;
            drawables_ = drawables;
        }

        float Renderer::getHeight() {
            float height = 0;
            for (const auto drawable : drawables_) {
                if (drawable->getHeight() > height) {
                    height = drawable->getHeight();
                }
            }

            return height;
        }

        float Renderer::getWidth() {
            float width = 0;
            for (const auto drawable : drawables_) {
                if (drawable->getWidth() > width) {
                    width = drawable->getWidth();
                }
            }

            return width;
        }

        void Renderer::setup() {
            for (size_t i = 0; i < drawables_.size(); i++) {
                drawables_[i]->setup();
            }

            channels = std::make_shared<syntheffect::graphics::PingPongBufferMap>(getWidth(), getHeight(), GL_RGB);
            for (size_t i = 0; i < drawables_.size(); i++) {
                channels->allocate(getChannelName(i));
                channels->allocate(getLastName(i));
            }

            channels->allocate(CHANNEL_OUT);
            channels->allocate(CHANNEL_LAST_OUT);

            auto builder = std::make_unique<syntheffect::patch::PatchBuilder>();
            patch_ = builder->build(patch_path_, channels);
            if (!patch_) {
                throw std::runtime_error("Failed to build patch '" + patch_path_ + "'");
            }
        }

        bool Renderer::isReady() {
            return ready_;
        }

        bool Renderer::update(std::shared_ptr<graphics::Params> params, float t) {
            for (const auto drawable : drawables_) {
                drawable->update(params, t);
                if (!drawable->isReady()) {
                    return true;
                }
            }

            for (const auto drawable : drawables_) {
                if (!drawable->isReady()) {
                    return true;
                }
            }

            patch_->setEffectParams(params);

            // Read the video frame
            // Write channelN to last-channelN
            for (size_t i = 0; i < drawables_.size(); i++) {
                std::shared_ptr<graphics::PingPongBuffer> chan = channels->get(getChannelName(i));
                chan->begin();
                ofClear(0);
                drawables_[i]->drawScaleCenter(chan->getWidth(), chan->getHeight());
                chan->end();
            }

            // Apply effects/write to channels
            patch_->drawTo(channels, t);

            // Write out channel to last-out channel
            channels->get(CHANNEL_OUT)->drawTo(channels->get(CHANNEL_LAST_OUT));

            // Write channelN to last-channelN
            for (size_t i = 0; i < drawables_.size(); i++) {
                channels->get(getChannelName(i))->drawTo(channels->get(getLastName(i)));
            }

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

        std::string Renderer::getLastName(int i) {
            return "last-channel" + std::to_string(i + 1);
        }

        std::string Renderer::getChannelName(int i) {
            return "channel" + std::to_string(i + 1);
        }

        void Renderer::draw(float x, float y, float width, float height) {
            channels->get(CHANNEL_OUT)->drawable()->draw(x, y, width, height);
        }
    }
}

#undef DEBUG_CHANNEL_ACCESS
