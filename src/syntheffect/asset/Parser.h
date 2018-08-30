#pragma once

#include <vector>

#include "ofXml.h"

#include "syntheffect/asset/AssetManager.h"
#include "syntheffect/xml/Util.h"

namespace syntheffect {
    namespace asset {
        class Parser {
            public:
                static void addAssets(AssetManager& m, std::string path);

                static void addAssetGroup(AssetManager& m, const ofXml& xml, std::string settings_path);
                static void addImageAsset(AssetManager& m, const ofXml& xml, std::string group, std::string root);
                static void addVideoAsset(AssetManager& m, const ofXml& xml, std::string group, std::string root);
                static void addWebcamAsset(AssetManager& m, const ofXml& xml, std::string group);
        };
    }
}
