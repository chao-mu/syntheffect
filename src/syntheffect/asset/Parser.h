#pragma once

#include <vector>

#include "ofXml.h"

#include "syntheffect/asset/Video.h"
#include "syntheffect/asset/Image.h"
#include "syntheffect/asset/Webcam.h"
#include "syntheffect/asset/AssetManager.h"
#include "syntheffect/xml/Util.h"

namespace syntheffect {
    namespace asset {
        class Parser {
            public:
                static void addAssets(AssetManager& m, const std::string& path);

                static void addAssetGroup(AssetManager& m, const ofXml& xml, const std::string& settings_path);
                static std::shared_ptr<Image> parseImageAsset(AssetManager& m, const ofXml& xml, const std::string& root);
                static std::shared_ptr<Video> parseVideoAsset(AssetManager& m, const ofXml& xml, const std::string& root);
                static std::shared_ptr<Webcam> parseWebcamAsset(AssetManager& m, const ofXml& xml);
                static std::string joinIfRelative(const std::string& root, const std::string& path);
        };
    }
}
