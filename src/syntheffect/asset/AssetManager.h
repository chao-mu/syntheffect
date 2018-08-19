#pragma once

#include <set>
#include <map>
#include <vector>
#include <memory>

#include "syntheffect/settings/AssetGroupSettings.h"

#include "syntheffect/asset/Asset.h"

namespace syntheffect {
    namespace asset {
        class AssetManager {
            public:
                void setAssets(const std::vector<settings::AssetGroupSettings>& asset_groups);

                std::vector<std::shared_ptr<Asset>> getActiveAssets();

                void update(float t);

                bool isReady();
                bool isFinished();

                void triggerAssetGroup(std::string name);

            private:
                std::map<std::string, std::map<std::string, std::vector<std::shared_ptr<Asset>>>> assets_by_group_;
                std::map<std::string, std::map<std::string, int>> asset_cursors_;
                std::map<std::string, std::shared_ptr<Asset>> active_assets_;
        };
    }
}
