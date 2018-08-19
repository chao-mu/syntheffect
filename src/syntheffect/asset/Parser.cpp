#include "syntheffect/asset/Parser.h"

#include "syntheffect/xml/Util.h"

namespace syntheffect {
    namespace asset {
         std::vector<settings::AssetGroupSettings> Parser::parseAssets(std::string path) {
            ofXml xml;

            if (!xml.load(path)) {
               throw std::runtime_error("Unable to load asset settings file " + path);
            }

            ofXml::Search assets_search = xml.find("//assets");
            if (assets_search.empty()) {
                throw std::runtime_error(path + " is missing <assets> section");
            }

            std::vector<settings::AssetGroupSettings> asset_groups;
            // Iterate over pipeline
            for (const auto& child : assets_search.getFirst().getChildren()) {
               std::string el_name = child.getName();
                if (el_name == "assetGroup") {
                    asset_groups.push_back(parseAssetGroup(child, path));
                } else {
                    throw std::runtime_error("Expecting assetGroup, got <" + el_name + ">");
                }
            }

            return asset_groups;
         }

         settings::AssetGroupSettings Parser::parseAssetGroup(const ofXml& xml, std::string settings_path) {
             settings::AssetGroupSettings group;

             group.name = xml::Util::getAttribute<std::string>(xml, "name", false, "");
             group.trigger = xml::Util::getAttribute<std::string>(xml, "trigger", false, "");

             std::string root = ofFilePath::getEnclosingDirectory(settings_path);

             for (const auto& child : xml.getChildren()) {
                 settings::AssetSettings asset_settings;

                 std::string el_name = child.getName();
                 if (el_name == "webcam") {
                     asset_settings = parseWebcamAsset(child);
                 } else if (el_name == "image") {
                     asset_settings = parseImageAsset(child, root);
                 } else if (el_name == "video") {
                     asset_settings = parseVideoAsset(child, root);
                 } else {
                     throw std::runtime_error("Expected <image>, <video>, or <webcam>, got <" + el_name + ">");
                 }
                 group.assets.push_back(asset_settings);
             }

             return group;
         }

         settings::AssetSettings Parser::parseWebcamAsset(const ofXml& xml) {
             settings::AssetSettings asset;

             asset.name = xml::Util::getAttribute<std::string>(xml, "name", true, "");
             asset.type = settings::WebcamType;

             return asset;
         }

         settings::AssetSettings Parser::parseVideoAsset(const ofXml& xml, std::string root) {
             settings::AssetSettings asset;

             asset.name = xml::Util::getAttribute<std::string>(xml, "name", true, "");
             asset.path = ofFilePath::join( root, xml::Util::getAttribute<std::string>(xml, "path", true, ""));
             asset.type = settings::VideoType;

             return asset;
         }

         settings::AssetSettings Parser::parseImageAsset(const ofXml& xml, std::string root) {
             settings::AssetSettings asset;

             asset.name = xml::Util::getAttribute<std::string>(xml, "name", true, "");
             asset.path = ofFilePath::join( root, xml::Util::getAttribute<std::string>(xml, "path", true, ""));
             asset.type = settings::ImageType;

             return asset;
         }
    }
}
