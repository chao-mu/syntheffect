#include "syntheffect/input/InputManager.h"

#include "GLFW/glfw3.h"

#include "ofEventUtils.h"

#include "ofLog.h"

namespace syntheffect {
    namespace input {
        JoystickID InputManager::addJoystick(JoystickSettings js) {
            JoystickID id = nextID();
            joysticks_[id] = std::make_shared<Joystick>(js);

            return id;
        }

        void InputManager::addTriggerAssetGroup(JoystickID joy_id, std::string name, std::string control) {
            asset_triggers_[joy_id][control] = name;
        }

        void InputManager::addTriggerParamSet(JoystickID joy_id, std::string control, settings::ParamSettings param) {
            param_triggers_[joy_id][control].push_back(param);
        }

        void InputManager::assignJoysticks() {
            // Handy lookup from glfw joystick id to our ids
            std::map<int, JoystickID> our_ids;
            for (const auto& kv : glfw_ids_) {
                our_ids[kv.second] = kv.first;
            }

            // Build a collection of available devices
            std::vector<int> available_devices;
            for (int i=GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++) {
                if (glfwJoystickPresent(i) == GLFW_TRUE) {
                    available_devices.push_back(i);
                }
            }

            // Map any disconnected/unassigned joysticks
            for (auto& kv : joysticks_) {
                JoystickID id = kv.first;
                std::shared_ptr<Joystick> joy = kv.second;

                // See if disconnected/unassigned
                if ((glfw_ids_.count(id) > 0 && glfwJoystickPresent(glfw_ids_[id]) != GLFW_TRUE) || glfw_ids_.count(id) == 0) {
                    for (const auto& glfw_id : available_devices) {
                        if (joy->isCompatible(glfw_id)) {
                            glfw_ids_[id] = glfw_id;
                        }
                    }
                }
            }
        }

        void InputManager::update(float t) {
            assignJoysticks();

            std::map<JoystickID, std::map<std::string, JoystickPress>> seen_presses;
            for (auto& kv : joysticks_) {
                JoystickID id = kv.first;
                std::shared_ptr<Joystick> joy = kv.second;

                if (!glfw_ids_.count(id)) {
                    continue;
                }

                int glfw_id = glfw_ids_[id];
                if (glfwJoystickPresent(glfw_id) == GLFW_FALSE) {
                    continue;
                }

                std::map<std::string, JoystickPress> presses = joy->getPresses(t, glfw_id);
                for (const auto& kv : presses) {
                    std::string control = kv.first;
                    JoystickPress press = kv.second;

                    if (press.first && asset_triggers_.count(id) && asset_triggers_.at(id).count(control)) {
                        ofNotifyEvent(asset_trigger_events, asset_triggers_.at(id).at(control));
                    }

                    seen_presses[id][control] = press;
                }
            }

            for (auto& kv : param_triggers_) {
                JoystickID joy_id = kv.first;
                for (auto& control_params : kv.second) {
                    std::string control = control_params.first;
                    std::vector<settings::ParamSettings>& params = control_params.second;

                    if (seen_presses.count(joy_id) && seen_presses.at(joy_id).count(control)) {
                        for (auto& p : params) {
                            if (p.variable_value == "press_time") {
                                float v = seen_presses.at(joy_id).at(control).pressed_time;
                                ofNotifyEvent(param_trigger_events, p.withValue(v));
                            } else {
                                ofNotifyEvent(param_trigger_events, p);
                            }
                        }
                    } else {
                        for (auto& p : params) {
                            ofNotifyEvent(param_trigger_events, p.asDefault());
                        }
                    }
                }
            }
        }

        JoystickID InputManager::nextID() {
            return last_id_++;
        }
    }
}
