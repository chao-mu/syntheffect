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

            for (auto const& child : xml.find("//inputs/*")) {
                std::string el_name = child.getName();
                if (el_name == "joystick") {
                    addJoystick(manager, child);
                } else {
                    std::runtime_error("Expected <joystick>, got <" + el_name + ">");
                }
            }
        }

        void Parser::addControl(InputManager& manager, const ofXml& xml, JoystickID joy_id) {
            std::string control = xml::Util::getAttribute<std::string>(xml, "control", true, "");
            ofLog() << control;

            for (const ofXml& child : xml.getChildren()) {
                std::string el_name = child.getName();

                if (el_name == "setAssetGroup") {
                    std::string target = xml::Util::getAttribute<std::string>(child, "target", true, "");

                    manager.addSetAssetGroup(joy_id, control, target);
                } else if(el_name == "setParam") {
                    param::Param p = param::Parser::parseParam(child, true);
                    manager.addSetParam(joy_id, control, p);
                } else {
                    throw std::runtime_error("Expected <setAssetGroup> or <setParam> got <" + el_name + ">");
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

                if (el_name == "control") {
                    addControl(manager, child, joy_id);
                } else {
                    throw std::runtime_error("Expected <control> got <" + el_name + ">");
                }
            }
        }
    }
}
