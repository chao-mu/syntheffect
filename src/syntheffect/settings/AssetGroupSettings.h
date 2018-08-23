#pragma once

#include <vector>

#include "syntheffect/settings/AssetSettings.h"

namespace syntheffect {
    namespace settings {
        struct AssetGroupSettings {
            std::string name;
            std::vector<AssetSettings> assets;
            std::string trigger;
        };
    }
}
