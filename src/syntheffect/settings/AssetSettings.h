#pragma once

#include <string>

namespace syntheffect {
    namespace settings {
        enum AssetType {
            ImageType,
            VideoType,
            WebcamType,
        };

        struct AssetSettings {
            std::string name;
            std::string path;
            AssetType type;
        };
    }
}
