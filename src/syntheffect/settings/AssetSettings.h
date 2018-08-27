#pragma once

#include <string>

#include "ofVideoBaseTypes.h"

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
            ofLoopType loop = OF_LOOP_NORMAL;
            int device_id;
        };
    }
}
