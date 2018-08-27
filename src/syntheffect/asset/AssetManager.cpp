#include "syntheffect/asset/AssetManager.h"

#include "ofUtils.h"

#include "syntheffect/asset/Video.h"
#include "syntheffect/asset/Image.h"
#include "syntheffect/asset/Webcam.h"

namespace syntheffect {
    namespace asset {

        void AssetManager::setAssets(const std::vector<settings::AssetGroupSettings>& asset_groups) {
            std::set<std::string> asset_names;
            for (const auto& group_settings : asset_groups) {
                for (const auto& asset_settings : group_settings.assets) {
                    std::string name = asset_settings.name;

                    std::shared_ptr<Asset> asset = nullptr;
                    switch (asset_settings.type) {
                        case settings::ImageType:
                            asset = std::make_shared<Image>(name, asset_settings.path);
                            break;
                        case settings::WebcamType:
                            asset = std::make_shared<Webcam>(name, asset_settings.device_id);
                            break;
                        case settings::VideoType:
                            asset = std::make_shared<Video>(name, asset_settings.path, 0, asset_settings.loop);
                            break;
                        default:
                            throw std::runtime_error("Internal error. Unexpected asset type");
                    }

                    assets_by_group_[group_settings.trigger][name].push_back(asset);
                    if (!asset_cursors_[group_settings.trigger].count(name)) {
                        asset_cursors_[group_settings.trigger][name] = 0;
                    }

                    if (!active_assets_.count(name)) {
                        active_assets_[name] = asset;
                    }

                    asset->setup();
                }
            }
        }

        void AssetManager::triggerAssetGroup(std::string group) {
            if (!assets_by_group_.count(group)) {
                throw std::out_of_range("Asset group '" + group + "' does not exist");

            }

            // Deactivate existing assets and activate new ones
            for (const auto& kv : assets_by_group_.at(group)) {
                const auto& asset_name = kv.first;
                auto& siblings = kv.second;

                int index = (asset_cursors_.at(group).at(asset_name) + 1) % siblings.size();
                std::shared_ptr<Asset> next_asset = siblings.at(index);
                active_assets_[asset_name] = next_asset;

                // Activate something different next time
                asset_cursors_[group][asset_name] = index;
            }
        }

        void AssetManager::update(float t) {
            // Update assets
            for (auto asset : getActiveAssets()) {
                asset->update(t);
            }
        }

        bool AssetManager::isReady() {
            for (auto asset : getActiveAssets()) {
                if (!asset->isReady()) {
                    return false;
                }
            }

            return true;
        }

        std::vector<std::shared_ptr<Asset>> AssetManager::getActiveAssets() {
            std::vector<std::shared_ptr<Asset>> actives;
            for (const auto& kv : active_assets_) {
                actives.push_back(kv.second);
            }

            return actives;
        }

        bool AssetManager::isFinished() {
            // TODO: Find a way to implement this meaningfully
           return false;
        }
    }
}

