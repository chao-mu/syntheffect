#include "syntheffect/asset/Parser.h"

#include <boost/algorithm/string.hpp>

#include "ofVideoBaseTypes.h"

#include "syntheffect/xml/Util.h"

namespace syntheffect {
    namespace asset {
         void Parser::addAssets(AssetManager& m, std::string path) {
            ofXml xml;

            if (!xml.load(path)) {
               throw std::runtime_error("Unable to load asset settings file " + path);
            }

            ofXml::Search assets_search = xml.find("//assets");
            if (assets_search.empty()) {
                throw std::runtime_error(path + " is missing <assets> section");
            }

            // Iterate over pipeline
            for (const auto& child : assets_search.getFirst().getChildren()) {
               std::string el_name = child.getName();
                if (el_name == "assetGroup") {
                    addAssetGroup(m, child, path);
                } else {
                    throw std::runtime_error("Expecting assetGroup, got <" + el_name + ">");
                }
            }
         }

         void Parser::addAssetGroup(AssetManager& m, const ofXml& xml, std::string settings_path) {
             std::string group = xml::Util::getAttribute<std::string>(xml, "name", false, "");
             std::string root = ofFilePath::getEnclosingDirectory(settings_path);

             for (const auto& child : xml.getChildren()) {
                 std::string el_name = child.getName();
                 std::shared_ptr<Asset> asset;

                 if (el_name == "webcam") {
                     asset = parseWebcamAsset(m, child);
                 } else if (el_name == "image") {
                     asset = parseImageAsset(m, child, root);
                 } else if (el_name == "video") {
                     asset = parseVideoAsset(m, child, root);
                 } else {
                     throw std::runtime_error("Expected <image>, <video>, or <webcam>, got <" + el_name + ">");
                 }

                 asset->setGroup(group);
                 asset->setStack(child.getAttribute("stack").getValue());

                 m.addAsset(asset);
             }
         }

         std::shared_ptr<Webcam> Parser::parseWebcamAsset(AssetManager& m, const ofXml& xml) {
             std::string id = xml::Util::getAttribute<std::string>(xml, "id", true, "");
             int device_id = xml::Util::getAttribute<int>(xml, "device", false, 0);

             return std::make_shared<Webcam>(id, device_id);
         }

         std::shared_ptr<Video> Parser::parseVideoAsset(AssetManager& m, const ofXml& xml, std::string root) {
             std::string id = xml::Util::getAttribute<std::string>(xml, "id", true, "");
             std::string path = ofFilePath::join(root, xml::Util::getAttribute<std::string>(xml, "path", true, ""));
             float volume = xml::Util::getAttribute<float>(xml, "volume", false, 0);

             std::string loop_raw = xml::Util::getAttribute<std::string>(xml, "loop", false, "normal");
             ofLoopType loop_type;
             if (loop_raw == "normal") {
                 loop_type = OF_LOOP_NORMAL;
             } else if (loop_raw == "palindrome") {
                 loop_type = OF_LOOP_PALINDROME;
             } else if (loop_raw == "none") {
                 loop_type = OF_LOOP_NONE;
             } else {
                 throw std::runtime_error("Unexpected loop attribute value of '" + loop_raw + "', expected normal, palindrome, or none");
             }

             return std::make_shared<Video>(id, path, volume, loop_type);
         }

         std::shared_ptr<Image> Parser::parseImageAsset(AssetManager& m, const ofXml& xml, std::string root) {
             std::string id = xml::Util::getAttribute<std::string>(xml, "id", true, "");
             std::string path = ofFilePath::join( root, xml::Util::getAttribute<std::string>(xml, "path", true, ""));

             return std::make_shared<Image>(id, path);
         }
    }
}
