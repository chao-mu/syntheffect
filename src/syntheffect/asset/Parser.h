#pragma once

#include <vector>

#include "ofXml.h"

#include "syntheffect/settings/AssetGroupSettings.h"
#include "syntheffect/settings/AssetSettings.h"
#include "syntheffect/xml/Util.h"

namespace syntheffect {
    namespace asset {
        class Parser {
            public:
                static std::vector<settings::AssetGroupSettings> parseAssets(std::string path);

                static settings::AssetGroupSettings parseAssetGroup(const ofXml& xml, std::string settings_path);

                static settings::AssetSettings parseImageAsset(const ofXml& xml, std::string root);
                static settings::AssetSettings parseVideoAsset(const ofXml& xml, std::string root);
                static settings::AssetSettings parseWebcamAsset(const ofXml& xml);

        };
    }
}
