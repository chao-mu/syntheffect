#include "syntheffect/asset/AssetManager.h"

#include "ofUtils.h"

#include "syntheffect/asset/Video.h"
#include "syntheffect/asset/Image.h"
#include "syntheffect/asset/Webcam.h"

namespace syntheffect {
    namespace asset {
        void AssetManager::addAsset(std::string group, std::string stack, std::shared_ptr<Asset> asset) {
            std::string id = asset->getID();

            if (stack == "") {
                stack = id;
            }

            stacks_by_group_[group][stack].push_back(asset);
            if (!stack_cursors_[group].count(stack)) {
                stack_cursors_[group][stack] = 0;
            }

            if (!active_assets_.count(stack)) {
                active_assets_[stack] = id;
            }

            asset->setup();
        }

        void AssetManager::triggerAssetGroup(std::string group) {
            if (!stacks_by_group_.count(group)) {
                throw std::out_of_range("Asset group '" + group + "' does not exist");
            }

            // Deactivate existing assets and activate new ones
            for (const auto& kv : stacks_by_group_.at(group)) {
                const auto& stack_name = kv.first;
                auto& siblings = kv.second;

                int index = (stack_cursors_.at(group).at(stack_name) + 1) % siblings.size();
                active_assets_[stack_name] = siblings.at(index)->getID();

                // Activate something different next time
                stack_cursors_[group][stack_name] = index;
            }
        }

        void AssetManager::update(float t) {
            // Update assets
            for (auto asset : getAssets()) {
                asset->update(t);
            }
        }

        bool AssetManager::isReady() {
            for (auto asset : getAssets()) {
                if (!asset->isReady()) {
                    return false;
                }
            }

            return true;
        }

        std::map<std::string, std::string> AssetManager::getStackToAsset() {
            return active_assets_;
        }

        std::vector<std::shared_ptr<Asset>> AssetManager::getAssets() {
            std::vector<std::shared_ptr<Asset>> assets;
            for (const auto& kv : stacks_by_group_) {
                for (const auto& stack_and_assets : kv.second) {
                    for (auto asset : stack_and_assets.second) {
                        assets.push_back(asset);
                    }
                }
            }

            return assets;
        }

        bool AssetManager::isFinished() {
            // TODO: Find a way to implement this meaningfully
           return false;
        }
    }
}
