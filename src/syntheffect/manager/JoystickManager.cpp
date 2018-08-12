#include "syntheffect/manager/JoystickManager.h"

#include "syntheffect/safety.h"

#include "GLFW/glfw3.h"

namespace syntheffect {
    namespace manager {

        void JoystickManager::addJoystick(settings::JoystickSettings js) {
            joysticks_[nextID()] = std::make_shared<controller::Joystick>(js);
        }

        void JoystickManager::assignJoysticks() {
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
                std::shared_ptr<controller::Joystick> joy = kv.second;

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

        void JoystickManager::update(float t) {
            assignJoysticks();

            for (auto& kv : joysticks_) {
                JoystickID id = kv.first;
                std::shared_ptr<controller::Joystick> joy = kv.second;

                int glfw_id = safety::at(glfw_ids_, id, "GLFW ids when updating joysticks");
                if (glfwJoystickPresent(glfw_id) == GLFW_TRUE) {
                    joy->update(t, glfw_id);
                }
            }

        }

        void JoystickManager::copyTo(param::Params& params) {
            for (const auto& kv : joysticks_) {
                kv.second->copyTo(params);
            }
        }

        JoystickID JoystickManager::nextID() {
            return last_id_++;
        }
    }
}
