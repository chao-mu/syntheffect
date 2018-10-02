#include "syntheffect/asset/AssetManager.h"

#include "boost/filesystem.hpp"

#include "ofUtils.h"

#include "syntheffect/asset/Video.h"
#include "syntheffect/asset/Image.h"
#include "syntheffect/asset/Webcam.h"

namespace syntheffect {
    namespace asset {
        void AssetManager::setup(size_t buffer_size) {
            sound_analyzer_.setup(buffer_size);
        }

        void AssetManager::addAsset(std::shared_ptr<Asset> asset) {
            std::string id = asset->getID();
            std::string stack = asset->getStack();
            std::string group = asset->getGroup();

            if (assets_by_id_.count(id)) {
                throw std::runtime_error("Duplicate asset id: " + id);
            }

            if (assets_by_id_.count(stack) || assets_by_stack_.count(id)) {
                throw std::runtime_error("Asset id '" + id + "' collides with stack name");
            }

            assets_by_id_[id] = asset;
            if (!stack.empty()) {
                assets_by_stack_[stack].push_back(id);
                assets_by_group_and_stack_[group][stack].push_back(id);
            }

            assets_by_group_[group].push_back(id);
        }

        ofBaseSoundOutput& AssetManager::getSoundOutput() {
            return sound_output_;
        }

        void AssetManager::addDrawable(std::shared_ptr<Drawable> asset) {
            asset->setup();
            drawables_by_id_[asset->getID()] = asset;
            addAsset(asset);
        }

        void AssetManager::addAudio(std::shared_ptr<Audio> asset) {
            asset->setup();
            asset->getSoundObject().connectTo(sound_analyzer_).connectTo(sound_output_);
            audio_by_id_[asset->getID()] = asset;
            addAsset(asset);
        }

        void AssetManager::copyParamsTo(param::Params& p) {
            sound_analyzer_.copyParamsTo(p);
        }

        void AssetManager::activateAsset(const std::string& asset_id) {
            if (!assets_by_id_.count(asset_id)) {
                throw std::out_of_range("An asset with id '" + asset_id + "' does not exist");
            }

            auto asset = assets_by_id_.at(asset_id);
            if (asset->getStack().empty()) {
                asset->setActive(true);
                asset->play();
            } else {
                for (const auto& other_id : assets_by_stack_.at(asset->getStack())) {
                    auto other_asset = assets_by_id_.at(other_id);
                    if (other_id == asset_id) {
                        other_asset->setActive(true);
                        other_asset->play();
                    } else {
                        other_asset->setActive(false);
                        other_asset->pause();
                        other_asset->restart();
                    }
                }
            }
        }

        void AssetManager::activateGroup(const std::string& group) {
            if (!assets_by_group_and_stack_.count(group)) {
                throw std::out_of_range("Asset group '" + group + "' does not exist");
            }

            for (const auto& stack_and_assets : assets_by_group_and_stack_.at(group)) {
                activateAsset(stack_and_assets.second.front());
            }
        }

        void AssetManager::reorderActiveStack(const std::string& stack, std::function<void(std::vector<std::string>&)> f) {
            if (!assets_by_stack_.count(stack)) {
                throw std::out_of_range("Asset stack '" + stack + "' does not exist");
            }

            std::vector<std::string> ids = assets_by_stack_.at(stack);
            if (ids.size() < 2) {
                return;
            }

            for (const auto& asset_id : ids) {
                auto a = assets_by_id_.at(asset_id);
                if (a->isActive()) {
                    std::vector<std::string>& siblings = assets_by_group_and_stack_.at(a->getGroup()).at(a->getStack());
                    f(siblings);
                    activateAsset(siblings.front());
                    break;
                }
            }
        }

        void AssetManager::prevStackAsset(const std::string& stack) {
            reorderActiveStack(stack, [](std::vector<std::string>& siblings) {
                std::rotate(siblings.begin(), siblings.begin() + 1, siblings.end());
            });
        }


        void AssetManager::nextStackAsset(const std::string& stack) {
            reorderActiveStack(stack, [](std::vector<std::string>& siblings) {
                std::rotate(siblings.rbegin(), siblings.rbegin() + 1, siblings.rend());
            });
        }

        void AssetManager::shuffleActiveStack(const std::string& stack) {
            reorderActiveStack(stack, [](std::vector<std::string>& siblings) {
                std::random_shuffle(siblings.begin() + 1, siblings.end());
                std::rotate(siblings.begin(), siblings.begin(), siblings.end());
            });
        }

        void AssetManager::update(float t) {
            // Update assets
            for (const auto& kv : assets_by_id_) {
                kv.second->update(t);
            }

            // Ensure there assets with no stacks are active
            for (const auto& kv : assets_by_id_) {
                auto& asset = kv.second;
                if (asset->getStack().empty() && !asset->isActive()) {
                    activateAsset(asset->getID());
                }
            }

            // Ensure there is an asset active for each stack
            for (const auto& kv : assets_by_stack_) {
                auto asset_ids = kv.second;
                auto asset = assets_by_id_.at(asset_ids.front());
                if (!asset->isActive()) {
                    activateAsset(asset->getID());
                }
            }
        }

        bool AssetManager::isReady() {
            for (const auto& kv : assets_by_id_) {
                if (!kv.second->isReady()) {
                    return false;
                }
            }

            return true;
        }

        std::vector<std::shared_ptr<Drawable>> AssetManager::getDrawables() {
            std::vector<std::shared_ptr<Drawable>> assets;
            for (const auto& kv : drawables_by_id_) {
                assets.push_back(kv.second);
            }

            return assets;
        }

        bool AssetManager::isFinished() {
            // TODO: Find a way to implement this meaningfully
           return false;
        }
    }
}
