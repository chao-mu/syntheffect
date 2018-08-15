#include "syntheffect/manager/AssetManager.h"

#include "ofGraphics.h"
#include "ofUtils.h"

#include <boost/algorithm/string.hpp>

#include "syntheffect/graphics/Video.h"
#include "syntheffect/graphics/Image.h"

#define CHANNEL_OUT "out"
#define LAST_NAME_SUFFIX "-last"

namespace syntheffect {
    namespace manager {
        AssetManager::AssetManager(int width, int height) : channels_(width, height, GL_RGBA) {

        }

        void AssetManager::setup() {
            channels_.allocate(CHANNEL_OUT);
            channels_.allocate(getLastName(CHANNEL_OUT));
        }

        void AssetManager::setAssets(const std::vector<settings::AssetGroupSettings>& asset_groups) {
            for (const auto& asset_group : asset_groups) {
                // Add drawables
                for (const auto& asset : asset_group.assets) {
                    std::shared_ptr<graphics::Drawable> drawable;
                    switch (asset.type) {
                        case settings::ImageType:
                            drawable = std::make_shared<graphics::Image>(asset.path);
                            break;
                        case settings::VideoType:
                            drawable = std::make_shared<graphics::Video>(asset.path, 0);
                            break;
                    }

                    drawable->setup();

                    drawable_names_[drawable] = asset.name;
                    if (active_drawables_.count(asset.name) == 0) {
                        active_drawables_[asset.name] = drawable;
                    }

                    if (asset_group.trigger != "") {
                        triggerable_drawables_[asset_group.trigger].push_back(drawable);
                    }

                    channels_.get_or_allocate(asset.name);
                    channels_.get_or_allocate(getLastName(asset.name));
                }
            }
        }

        void AssetManager::update(float t, param::Params& params) {
            // Save previous buffers
            for (const auto& buf_name : channels_.getKeys()) {
                // Save previous buffer
                if (boost::algorithm::ends_with(buf_name, LAST_NAME_SUFFIX)) {
                    continue;
                }

                std::string last_buf_name = getLastName(buf_name);
                std::shared_ptr<graphics::PingPongBuffer> chan = channels_.get(last_buf_name);
                chan->begin();
                ofClear(0);
                channels_.get(buf_name)->drawable()->draw(0, 0);
                chan->end();
            }

            // Elect active asssets
            for (const auto& trigger_and_drawables : triggerable_drawables_) {
                if (params.getBool(trigger_and_drawables.first)) {
                    for (auto drawable : trigger_and_drawables.second) {
                        active_drawables_[drawable_names_.at(drawable)] = drawable;
                    }
                }
            }

            // Write drawables to their destinations
            for (const auto& name_and_drawable : active_drawables_) {
                std::string name = name_and_drawable.first;
                std::shared_ptr<graphics::Drawable> drawable = name_and_drawable.second;
                drawable->update(t, params);

                std::shared_ptr<graphics::PingPongBuffer> chan = channels_.get(name);
                chan->begin();
                ofClear(0);
                drawable->drawScaleCenter(chan->getWidth(), chan->getHeight());
                chan->end();
            }

            // Set texture parameters
            for (const auto& buf_name : channels_.getKeys()) {
                auto chan = channels_.get(buf_name);
                params.setTexture(buf_name, [chan]() { return chan->drawable()->getTexture(); });
            }
        }

        bool AssetManager::isReady() {
            for (const auto& name_and_drawable : active_drawables_) {
                if (!name_and_drawable.second->isReady()) {
                    return false;
                }
            }

            return true;
        }

        bool AssetManager::isFinished() {
            // TODO: Find a way to implement this meaningfully
           return false;
        }

        graphics::PingPongBufferMap& AssetManager::getChannels() {
            return channels_;
        }


        std::string AssetManager::getLastName(std::string buf_name) {
            return buf_name + LAST_NAME_SUFFIX;
        }
    }
}

#undef CHANNEL_OUT
#undef CHANNEL_LAST_OUT
