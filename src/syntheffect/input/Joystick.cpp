#include "syntheffect/input/Joystick.h"

#include "GLFW/glfw3.h"

#include "ofLog.h"
#include "ofUtils.h"
#include "ofMath.h"

#define LOG_JOYSTICK false

#define AXIS_LOW -1
#define AXIS_HIGH 1

namespace syntheffect {
    namespace input {
        Joystick::Joystick(JoystickSettings settings) {
            settings_ = settings;
        }

        bool Joystick::isCompatible(int glfw_id) {
            return std::string(glfwGetJoystickName(glfw_id)).find(settings_.device) != std::string::npos;
        }

        bool Joystick::isAxisPressed(const float* axes, int i, int sibling) {
            float v = axes[i];
            float deadzone = settings_.deadzone;
            float neutral = settings_.getAxisNeutral(i);

            bool pressed = (v - neutral < -deadzone) || (v - neutral > deadzone);
            if (sibling >= 0) {
                pressed |= isAxisPressed(axes, sibling);
            }

            return pressed;
        }

        int Joystick::getStickSibling(int i) {
            if (settings_.stick_siblings.count(i) > 0) {
                return settings_.stick_siblings.at(i);
            }

            return -1;
        }

        std::map<std::string, JoystickPress> Joystick::getPresses(float t, int id) {
            std::map<std::string, JoystickPress> presses;

            int axes_count;
            const float* axes = glfwGetJoystickAxes(id, &axes_count);
            for (int i=0; i < axes_count; i++) {
                std::string name = settings_.getAxisName(i);
                if (name == "") {
                    continue;
                }

                float v = axes[i];
                float deadzone = settings_.deadzone;
                float neutral = settings_.getAxisNeutral(i);
                float adjusted_low = -1 + deadzone;
                float adjusted_high = 1 - deadzone;
                bool adjusted = false;
                bool pressed = isAxisPressed(axes, i, getStickSibling(i));
                if (pressed) {
                    if (v - neutral < -deadzone) {
                        v += deadzone;
                        if (v > adjusted_high) {
                            v = adjusted_high;
                        }

                        adjusted = true;
                    } else if (v - neutral > deadzone) {
                        v -= deadzone;
                        if (v < adjusted_low) {
                            v = adjusted_low;
                        }

                        adjusted = true;
                    } else {
                        v = neutral;
                    }
                    // Remap to -1 to 1
                    if (adjusted) {
                        v = ofMap(v, adjusted_low, adjusted_high, AXIS_LOW, AXIS_HIGH);
                    }
                }

                if (pressed) {
                    presses[name] = makePress(name, t);
                } else {
                    press_start_.erase(name);
                }

                if (LOG_JOYSTICK) {
                    ofLogNotice("Joystick", "axis=%d name=%s raw=%f translated=%f adjusted=%i pressed=%i", i, name.c_str(), axes[i], v, adjusted, pressed);
                }
            }

            int button_count;
            const unsigned char* buttons = glfwGetJoystickButtons(id, &button_count);
            for (int i=0; i < button_count; i++) {
                std::string name = settings_.getButtonName(i);
                if (name == "") {
                    continue;
                }

                bool pressed = buttons[i] == GLFW_PRESS;
                if (pressed) {
                    presses[name] = makePress(name, t);
                } else {
                    press_start_.erase(name);
                }

                if (LOG_JOYSTICK) {
                    ofLogNotice("Joystick", "button=%d name=%s pressed=%i", i, name.c_str(), pressed);
                }
            }

            return presses;
        }

        JoystickPress Joystick::makePress(std::string name, float t) {
            JoystickPress press;
            press.name = name;
            press.first = press_start_.count(name) == 0;

            if (!press_start_.count(name)) {
                press_start_[name] = t;
            }

            press.pressed_time = t - press_start_.at(name);

            return press;
        }
    }
}

#undef LOG_JOYSTICK
#undef AXIS_LOW
#undef AXIS_HIGH
