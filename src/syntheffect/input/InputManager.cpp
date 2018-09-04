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

        void InputManager::addControlMapping(JoystickID joy_id, ControlMapping mapping) {
            mappings_[joy_id].push_back(mapping);
        }

        void InputManager::assignJoysticks() {
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
                JoystickID joy_id = kv.first;
                std::shared_ptr<Joystick> joy = kv.second;

                if (!glfw_ids_.count(joy_id)) {
                    continue;
                }

                int glfw_id = glfw_ids_[joy_id];
                if (glfwJoystickPresent(glfw_id) == GLFW_FALSE) {
                    continue;
                }

                std::map<std::string, JoystickPress> presses = joy->getPresses(t, glfw_id);
                for (const auto& kv : presses) {
                    std::string control = kv.first;
                    JoystickPress press = kv.second;

                    seen_presses[joy_id][control] = press;
                }
            }

            for (auto& joy_and_mappings : mappings_) {
                JoystickID joy_id = joy_and_mappings.first;
                for (auto& mapping : joy_and_mappings.second) {
                    std::string control = mapping.control;

                    ControlState state;
                    state.mapping = mapping;
                    state.pressed = seen_presses.count(joy_id) && seen_presses.at(joy_id).count(control);
                    if (state.pressed) {
                        auto press = seen_presses.at(joy_id).at(control);
                        state.pressed_time = press.pressed_time;
                        state.first_press = press.first;
                    }

                    ofNotifyEvent(state_events, state, this);
                }
            }
        }

        JoystickID InputManager::nextID() {
            return last_id_++;
        }
    }
}
