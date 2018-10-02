#include "syntheffect/asset/Parser.h"

#include <boost/algorithm/string.hpp>

#include "ofVideoBaseTypes.h"

#include "syntheffect/xml/Util.h"

namespace syntheffect {
    namespace asset {
         void Parser::addAssets(AssetManager& m, const std::string& path) {
            ofXml xml;

            if (!xml.load(path)) {
               throw std::runtime_error("Unable to load asset settings file " + path);
            }

            ofXml::Search assets_search = xml.find("//assets");
            if (assets_search.empty()) {
                throw std::runtime_error(path + " is missing <assets> section");
            }

            xml = assets_search.getFirst();

            std::string root = xml::Util::getAttribute<std::string>(xml, "root", false, "");
            if (root.empty()) {
                root = ofFilePath::getEnclosingDirectory(path);
                ofLogNotice("assets::Parser", "root was empty: %s", root.c_str());
            } else if (!ofFilePath::isAbsolute(root)) {
                std::string proj_dir = ofFilePath::getEnclosingDirectory(path);
                root = ofFilePath::join(proj_dir, root);
            }

            // Iterate over pipeline
            for (const auto& child : xml.getChildren()) {
               std::string el_name = child.getName();
                if (el_name == "assetGroup") {
                    addAssetGroup(m, child, root);
                } else {
                    throw std::runtime_error("Expecting assetGroup, got <" + el_name + ">");
                }
            }
         }

         void Parser::addAssetGroup(AssetManager& m, const ofXml& xml, const std::string& root) {
             std::string group = xml::Util::getAttribute<std::string>(xml, "name", false, "");

             for (const auto& child : xml.getChildren()) {
                 std::string el_name = child.getName();

                 if (el_name == "webcam") {
                     addDrawable(m, child, group, parseWebcamAsset(m, child));
                 } else if (el_name == "image") {
                     addDrawable(m, child, group, parseImageAsset(m, child, root));
                 } else if (el_name == "video") {
                     addDrawable(m, child, group, parseVideoAsset(m, child, root));
                 } else if (el_name == "audio") {
                     addAudio(m, child, group, parseAudioFileAsset(m, child, root));
                 } else {
                     throw std::runtime_error("Expected <image>, <video>, or <webcam>, got <" + el_name + ">");
                 }
             }
         }

         void Parser::addDrawable(AssetManager& m, const ofXml& xml, const std::string& group, std::shared_ptr<Drawable> asset) {
                 asset->setGroup(group);
                 asset->setStack(xml.getAttribute("stack").getValue());

                 m.addDrawable(asset);
         }

         void Parser::addAudio(AssetManager& m, const ofXml& xml, const std::string& group, std::shared_ptr<Audio> asset) {
                 asset->setGroup(group);
                 asset->setStack(xml.getAttribute("stack").getValue());

                 m.addAudio(asset);
         }

         std::shared_ptr<Webcam> Parser::parseWebcamAsset(AssetManager& m, const ofXml& xml) {
             std::string id = xml::Util::getAttribute<std::string>(xml, "id", true, "");
             int device_id = xml::Util::getAttribute<int>(xml, "device", false, 0);

             return std::make_shared<Webcam>(id, device_id);
         }

         std::shared_ptr<Audio> Parser::parseAudioFileAsset(AssetManager& m, const ofXml& xml, const std::string& root) {
             std::string id = xml::Util::getAttribute<std::string>(xml, "id", true, "");
             std::string path = joinIfRelative(root, xml::Util::getAttribute<std::string>(xml, "path", true, ""));
             float volume = xml::Util::getAttribute<float>(xml, "volume", false, 1);
             bool loop = xml::Util::getAttribute<bool>(xml, "loop", false, false);

             return std::make_shared<Audio>(id, path, volume, loop);
         }

         std::shared_ptr<Video> Parser::parseVideoAsset(AssetManager& m, const ofXml& xml, const std::string& root) {
             std::string id = xml::Util::getAttribute<std::string>(xml, "id", true, "");
             std::string path = joinIfRelative(root, xml::Util::getAttribute<std::string>(xml, "path", true, ""));
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

         std::shared_ptr<Image> Parser::parseImageAsset(AssetManager& m, const ofXml& xml, const std::string& root) {
             std::string id = xml::Util::getAttribute<std::string>(xml, "id", true, "");
             std::string path = ofFilePath::join(root, xml::Util::getAttribute<std::string>(xml, "path", true, ""));

             return std::make_shared<Image>(id, path);
         }

         std::string Parser::joinIfRelative(const std::string& root, const std::string& path) {
             if (ofFilePath::isAbsolute(path)) {
                 return path;
             }

             return ofFilePath::join(root, path);
         }
    }
}
