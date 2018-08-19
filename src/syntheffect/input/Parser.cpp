#include "syntheffect/input/Parser.h"

#include "yaml-cpp/yaml.h"

#include "syntheffect/input/JoystickSettings.h"
#include "syntheffect/xml/Util.h"
#include "syntheffect/param/Parser.h"

namespace syntheffect {
    namespace input {
        void Parser::addInputs(InputManager& manager, std::string path) {
                    ofXml xml;

            if (!xml.load(path)) {
               throw std::runtime_error("Unable to load project settings file " + path);
            }

            ofXml::Search search = xml.find("//inputs");
            if (search.empty()) {
                throw std::runtime_error(path + " is missing <inputs> section");
            }

            for (auto const& child : search.getFirst().getChildren()) {
                std::string el_name = child.getName();
                if (el_name == "joystick") {
                    addJoystick(manager, child);
                } else {
                    std::runtime_error("Expected <joystick>, got <" + el_name + ">");
                }
            }
        }

        void Parser::addJoystick(InputManager& manager, const ofXml& xml) {
            JoystickSettings joystick;

            std::string settings_path = ofFilePath::getAbsolutePath(
                    ofFilePath::join("joysticks", xml::Util::getAttribute<std::string>(xml, "settings", true, ""))) + ".yaml";

            YAML::Node config = YAML::LoadFile(settings_path);

            joystick.device = config["device"].as<std::string>();
            joystick.deadzone = config["deadzone"].as<float>();

            for (YAML::const_iterator it=config["neutral"].begin(); it != config["neutral"].end(); ++it) {
                joystick.axis_neutrals[it->first.as<int>()] = it->second.as<float>();
            }

            for (YAML::const_iterator it=config["stick_siblings"].begin(); it != config["stick_siblings"].end(); ++it) {
                joystick.stick_siblings[it->first.as<int>()] = it->second.as<float>();
            }

            for (YAML::const_iterator it=config["axes"].begin(); it != config["axes"].end(); ++it) {
                joystick.axis_names[it->first.as<int>()] = it->second.as<std::string>();
            }

            for (YAML::const_iterator it=config["buttons"].begin(); it != config["buttons"].end(); ++it) {
                joystick.button_names[it->first.as<int>()] = it->second.as<std::string>();
            }

            JoystickID joy_id = manager.addJoystick(joystick);

            for (const ofXml& child : xml.getChildren()) {
                std::string el_name = child.getName();

                if (el_name == "triggerAssetGroup") {
                    std::string name = xml::Util::getAttribute<std::string>(child, "name", true, "");
                    std::string control = xml::Util::getAttribute<std::string>(child, "control", true, "");


                    manager.addTriggerAssetGroup(joy_id, name, control);
                } else if(el_name == "triggerSetParams") {
                    std::string control = xml::Util::getAttribute<std::string>(child, "control", true, "");
                    for (auto param_child : child.getChildren()) {
                        settings::ParamSettings p = param::Parser::parseParam(param_child, true);
                        manager.addTriggerParamSet(joy_id, control, p);
                    }
                } else {
                    throw std::runtime_error("Expected <triggerAssetGroup> or <triggerSetParam> got <" + el_name + ">");
                }
            }
        }
    }
}
