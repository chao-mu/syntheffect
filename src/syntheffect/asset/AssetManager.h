#ifndef SYNTHEFFECT_ASSET_ASSETMANAGER_H
#define SYNTHEFFECT_ASSET_ASSETMANAGER_H

#pragma once

#include <set>
#include <map>
#include <vector>
#include <memory>

#include "syntheffect/asset/Asset.h"

namespace syntheffect {
    namespace asset {
        class AssetManager {
            public:
                void addAsset(std::shared_ptr<Asset> asset);

                void update(float t);

                bool isReady();
                bool isFinished();

                void activateGroup(const std::string& group);
                void activateAsset(const std::string& asset_id);
                void shuffleActiveStack(const std::string& stack);
                void prevStackAsset(const std::string& stack);
                void nextStackAsset(const std::string& stack);
                void reorderActiveStack(const std::string& stack, std::function<void(std::vector<std::string>&)> f);

                std::vector<std::shared_ptr<Asset>> getAssets();
                std::map<std::string, std::string> getStackToAsset();

            private:
                std::map<std::string, std::shared_ptr<Asset>> assets_by_id_;
                std::map<std::string, std::map<std::string, std::vector<std::string>>> assets_by_group_and_stack_;
                std::map<std::string, std::vector<std::string>> assets_by_stack_;
                std::map<std::string, std::vector<std::string>> assets_by_group_;

                // stack to asset
                std::map<std::string, std::string> active_assets_;
        };
    }
}

#endif
