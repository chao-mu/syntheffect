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
                static void addAssets(AssetManager& m, std::string path);

                static void addAssetGroup(AssetManager& m, const ofXml& xml, std::string settings_path);
                static std::shared_ptr<Image> parseImageAsset(AssetManager& m, const ofXml& xml, std::string root);
                static std::shared_ptr<Video> parseVideoAsset(AssetManager& m, const ofXml& xml, std::string root);
                static std::shared_ptr<Webcam> parseWebcamAsset(AssetManager& m, const ofXml& xml);
        };
    }
}
