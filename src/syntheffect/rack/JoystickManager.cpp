#include "syntheffect/rack/JoystickManager.h"

#include "GLFW/glfw3.h"

namespace syntheffect {
    namespace rack {
        void JoystickManager::update(float /* t */) {
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
                            joy->connect(glfw_id);
                            glfw_ids_[id] = glfw_id;
                        }
                    }
                }
            }
        }


        void JoystickManager::addJoystick(std::shared_ptr<Joystick> joystick) {
            joysticks_[next_id_++] = joystick;
        }
    }
}
