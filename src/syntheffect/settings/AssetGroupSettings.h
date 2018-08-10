#pragma once

#include <vector>

#include "syntheffect/settings/AssetSettings.h"
#include "syntheffect/settings/ParamSettings.h"

namespace syntheffect {
    namespace settings {
        struct AssetGroupSettings {
            std::string name;
            std::vector<AssetSettings> assets;
            settings::ParamSettings trigger;
        };
    }
}
