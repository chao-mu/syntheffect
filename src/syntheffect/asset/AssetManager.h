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
                void addAsset(std::string group, std::string stack, std::shared_ptr<Asset> asset);

                void update(float t);

                bool isReady();
                bool isFinished();

                void triggerAssetGroup(std::string name);

                std::vector<std::shared_ptr<Asset>> getAssets();
                std::map<std::string, std::string> getStackToAsset();

            private:
                std::map<std::string, std::map<std::string, std::vector<std::shared_ptr<Asset>>>> stacks_by_group_;
                std::map<std::string, std::map<std::string, int>> stack_cursors_;
                // stack to asset
                std::map<std::string, std::string> active_assets_;
                std::map<std::string, std::string> asset_names_;
        };
    }
}
