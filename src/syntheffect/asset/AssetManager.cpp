#include "syntheffect/asset/AssetManager.h"

#include "ofUtils.h"

#include "syntheffect/asset/Video.h"
#include "syntheffect/asset/Image.h"
#include "syntheffect/asset/Webcam.h"

namespace syntheffect {
    namespace asset {
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

            if (assets_by_stack_[stack].empty() || stack.empty()) {
                asset->setActive(true);
            }

            assets_by_id_[id] = asset;
            assets_by_stack_[stack].push_back(id);
            assets_by_group_[group].push_back(id);
            assets_by_group_and_stack_[group][stack].push_back(id);

            asset->setup();
        }

        void AssetManager::activateAsset(const std::string& asset_id) {
            if (!assets_by_id_.count(asset_id)) {
                throw std::out_of_range("An asset with id '" + asset_id + "' does not exist");
            }

            auto asset = assets_by_id_.at(asset_id);
            if (asset->getStack().empty()) {
                asset->setActive(true);
            } else {
                for (const auto& other_id : assets_by_stack_.at(asset->getStack())) {
                    assets_by_id_.at(other_id)->setActive(other_id == asset_id);
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

        std::vector<std::shared_ptr<Asset>> AssetManager::getAssets() {
            std::vector<std::shared_ptr<Asset>> assets;
            for (const auto& kv : assets_by_id_) {
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
