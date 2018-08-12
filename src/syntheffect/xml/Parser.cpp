#include "syntheffect/xml/Parser.h"

#include <cmath>
#include <string>

#include "yaml-cpp/yaml.h"
#include "boost/lexical_cast.hpp"

#include "ofMath.h"
#include "ofFileUtils.h"

#include "syntheffect/xml/Util.h"

#define DEFAULT_FRAG "Passthrough"
#define DEFAULT_VERT "Passthrough"

namespace syntheffect {
    namespace xml {
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
                  addAssetGroup(child, path, asset_groups);
                } else {
                    throw std::runtime_error("Expecting assetGroup, got <" + el_name + ">");
                }
            }

            return asset_groups;
         }

         settings::ProjectSettings Parser::parseProject(std::string path) {
            ofXml xml;

            if (!xml.load(path)) {
               throw std::runtime_error("Unable to load project settings file " + path);
            }

            ofXml::Search search = xml.find("//project");
            if (search.empty()) {
                throw std::runtime_error(path + " is missing <project> section");
            }

            settings::ProjectSettings proj;

            xml = search.getFirst();
            proj.assets_path = ofFilePath::join(
                    ofFilePath::getEnclosingDirectory(path),
                    Util::getAttribute<std::string>(xml, "assets", false, "assets.xml"));

            proj.asset_groups = parseAssets(proj.assets_path);

            proj.pipelines_path = ofFilePath::join(
                    ofFilePath::getEnclosingDirectory(path),
                    Util::getAttribute<std::string>(xml, "pipelines", false, "pipelines.xml"));

            proj.pipelines = parsePipelines(proj.pipelines_path);

            for (const auto& child : xml.getChildren()) {
               std::string el_name = child.getName();
               if (el_name == "joysticks") {
                   for (const auto& joysticks_child : child.getChildren()) {
                       if (joysticks_child.getName() == "joystick") {
                           addJoystick(joysticks_child, path, proj.joysticks);
                       } else {
                           throw std::runtime_error("Expected <joystick> got <" + joysticks_child.getName() + ">");
                       }
                   }
               } else {
                    throw std::runtime_error("Expected <joysticks> got <" + el_name + ">");
               }
            }

            return proj;
         }

         void Parser::addJoystick(const ofXml& xml, std::string, std::vector<settings::JoystickSettings>& joysticks) {
             settings::JoystickSettings joystick;
             joystick.prefix = Util::getAttribute<std::string>(xml, "prefix", false, "");

             std::string settings_path = ofFilePath::getAbsolutePath(
                     ofFilePath::join("joysticks", Util::getAttribute<std::string>(xml, "settings", true, ""))) + ".yaml";

             ofLogNotice("Parser", settings_path);
             YAML::Node config = YAML::LoadFile(settings_path);

             joystick.device = config["device"].as<std::string>();

             joystick.deadzone = config["deadzone"].as<float>();

             for(YAML::const_iterator it=config["neutral"].begin(); it != config["neutral"].end(); ++it) {
                 joystick.axis_neutrals[it->first.as<int>()] = it->second.as<float>();
             }

             for(YAML::const_iterator it=config["stick_siblings"].begin(); it != config["stick_siblings"].end(); ++it) {
                 joystick.stick_siblings[it->first.as<int>()] = it->second.as<float>();
             }

             for(YAML::const_iterator it=config["axes"].begin(); it != config["axes"].end(); ++it) {
                 ofLogNotice("Parser", "%d=%s", it->first.as<int>(), it->second.as<std::string>().c_str());
                 joystick.axis_names[it->first.as<int>()] = it->second.as<std::string>();
             }

             for(YAML::const_iterator it=config["buttons"].begin(); it != config["buttons"].end(); ++it) {
                 ofLogNotice("Parser", "(button) %d=%s", it->first.as<int>(), it->second.as<std::string>().c_str());
                 joystick.button_names[it->first.as<int>()] = it->second.as<std::string>();
             }

             joysticks.push_back(joystick);
         }

         void Parser::addAssetGroup(const ofXml& xml, std::string path, std::vector<settings::AssetGroupSettings>& asset_groups) {
             settings::AssetGroupSettings group;

             group.name = Util::getAttribute<std::string>(xml, "name", true, "");

             for (const auto& child : xml.getChildren()) {
                std::string el_name = child.getName();

                if (el_name == "asset") {
                    addAsset(child, path, group);
                } else if (el_name == "trigger") {
                    group.trigger = parseParam(child, false);
                } else {
                    throw std::runtime_error("Expected <asset> or <trigger>, got <" + el_name + ">");
                }
            }

             asset_groups.push_back(group);
         }

         void Parser::addAsset(const ofXml& xml, std::string path, settings::AssetGroupSettings& group) {
             settings::AssetSettings asset;

             asset.name = Util::getAttribute<std::string>(xml, "name", true, "");
             asset.path = ofFilePath::join(
                     ofFilePath::getEnclosingDirectory(path),
                     Util::getAttribute<std::string>(xml, "path", true, ""));

             std::string type_raw = Util::getAttribute<std::string>(xml, "type", true, "");
             if (type_raw == "image") {
                 asset.type = settings::ImageType;
             } else if (type_raw == "video") {
                 asset.type = settings::VideoType;
             } else {
                 throw std::runtime_error("invalid asset type of '" + type_raw + "' specified");
             }

             group.assets.push_back(asset);
         }

         settings::ParamSettings Parser::parseParam(const ofXml& xml, bool require_name) {
            std::string name = Util::getAttribute<std::string>(xml, "name", require_name, "");

            settings::ParamSettings p;
            p.name = name;

            std::string value = xml.getAttribute("value").getValue();
            try {
                p.value = boost::lexical_cast<float>(value);
            } catch (const boost::bad_lexical_cast &e) {
                p.variable_value = value;
            }

            std::string cast = xml.getAttribute("cast").getValue();
            if (cast == "round-int") {
                p.cast = settings::RoundIntCast;
            } else if (cast == "negative-is-true") {
                p.cast = settings::NegativeIsTrueCast;
            } else if (cast == "positive-is-true") {
                p.cast = settings::PositiveIsTrueCast;
            } else if (cast == "") {
                p.cast = settings::NoCast;
            } else {
                throw std::runtime_error("cast attribute missing or invalid in <param>");
            }

            std::string func = xml.getAttribute("func").getValue();
            if (func == "identity" || func == "") {
                p.func = settings::IdentityFunc;
            } else if (func == "noise") {
                p.func = settings::NoiseFunc;
            } else if (func == "cos") {
                p.func = settings::CosFunc;
            } else if (func == "sin") {
                p.func = settings::SinFunc;
            } else {
                throw std::runtime_error("type attribute missing or invalid in <param>");
            }

            std::string low = xml.getAttribute("low").getValue();
            if (low != "") {
                p.low = xml.getAttribute("low").getFloatValue();
            }

            std::string high = xml.getAttribute("high").getValue();
            if (high != "") {
                p.high = xml.getAttribute("high").getFloatValue();
            }

            std::string freq = xml.getAttribute("freq").getValue();
            if (freq != "") {
                p.freq = xml.getAttribute("freq").getFloatValue();
            }

            std::string shift = xml.getAttribute("shift").getValue();
            if (shift != "") {
                p.shift = xml.getAttribute("shift").getFloatValue();
            }

            return p;
         }

         std::vector<settings::PipelineSettings> Parser::parsePipelines(std::string path) {
            ofXml xml;

            if (!xml.load(path)) {
               throw std::runtime_error("Unable to load pipelines file " + path);
            }

            ofXml::Search pipelines_search = xml.find("//pipelines");
            if (pipelines_search.empty()) {
                throw std::runtime_error(path + " is missing <pipelines> section");
            }

            std::vector<settings::PipelineSettings> pipelines;
            // Iterate over pipeline
            for (const auto& child : pipelines_search.getFirst().getChildren()) {
               std::string el_name = child.getName();
                if (el_name == "pipeline") {
                  addPipeline(child, pipelines);
                } else {
                    throw std::runtime_error("Expecting pipeline, got <" + el_name + ">");
                }
            }

            return pipelines;
         }

        void Parser::addPipeline(const ofXml& xml, std::vector<settings::PipelineSettings>& pipelines) {
            std::string in = xml.getAttribute("in").getValue();
            if (in.empty()) {
                throw std::runtime_error("Missing 'in' attribute in pipeline element");
            }

            std::string out = xml.getAttribute("out").getValue();
            if (out.empty()) {
                throw std::runtime_error("Missing 'out' attribute in pipeline element");
            }

            settings::PipelineSettings pipeline;
            pipeline.in = in;
            pipeline.out = out;

            // Iterate over pipeline
            for (const auto& child : xml.getChildren()) {
                std::string el_name = child.getName();

                if (el_name == "shader") {
                    addShader(child, pipeline);
                } else {
                    throw std::runtime_error("Expected <shader>, got <" + el_name + ">");
                }
            }

            pipelines.push_back(pipeline);
        }

        void Parser::addShader(const ofXml& xml, settings::PipelineSettings& pipeline) {
            std::string frag = xml.getAttribute("frag").getValue();
            if (frag.empty()) {
                frag = DEFAULT_FRAG;
            }

            std::string vert = xml.getAttribute("vert").getValue();
            if (vert.empty()) {
                vert = DEFAULT_VERT;
            }

            settings::ShaderSettings shader;
            shader.frag = frag;
            shader.vert = vert;

            for (const auto& child : xml.find("*")) {
                addShaderParam(child, shader);
            }

            pipeline.shaders.push_back(shader);
        }

        void Parser::addShaderParam(const ofXml& xml, settings::ShaderSettings& shader) {
            std::string param_el = xml.getName();

            if (param_el == "paramTexture") {
                std::string name = xml.getAttribute("name").getValue();
                std::string value = xml.getAttribute("value").getValue();

                shader.texture_params[name] = value;
                return;
            } else if (param_el != "param") {
                throw std::runtime_error("Expected <param>, got <" + param_el + ">");
            }

            shader.params.push_back(parseParam(xml));
        }
    }
}

#undef DEFAULT_FRAG
#undef DEFAULT_VERT
