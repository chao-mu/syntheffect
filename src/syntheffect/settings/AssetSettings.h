#pragma once

#include <string>

namespace syntheffect {
    namespace settings {
        enum AssetType {
            ImageType,
            VideoType
        };

        struct AssetSettings {
            std::string name;
            std::string path;
            AssetType type;
        };
    }
}
