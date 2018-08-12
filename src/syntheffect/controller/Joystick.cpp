#include "syntheffect/controller/Joystick.h"

#include "GLFW/glfw3.h"

#include "ofLog.h"
#include "ofUtils.h"
#include "ofMath.h"

#define LOG_JOYSTICK true

#define AXIS_LOW -1
#define AXIS_HIGH 1

namespace syntheffect {
    namespace controller {
        Joystick::Joystick(settings::JoystickSettings settings) {
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

        void Joystick::update(float t, int id) {
            for (auto& kv : settings_.button_names) {
                ofLogNotice("Joystick", "%i=%s", kv.first, kv.second.c_str());
            }
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

                params_.set(settings::ParamSettings::floatValue(name, v, AXIS_LOW, AXIS_HIGH));
                setPressed(name, pressed, t);

                if (LOG_JOYSTICK) {
                    ofLogNotice("Joystick", "axis=%d name=%s raw=%f translated=%f adjusted=%i", i, name.c_str(), axes[i], v, adjusted);
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
                setPressed(name, pressed, t);
                params_.set(settings::ParamSettings::boolValue(name, pressed));

                if (LOG_JOYSTICK) {
                    ofLogNotice("Joystick", "button=%d name=%s pressed=%i", i, name.c_str(), pressed);
                }
            }
        }

        void Joystick::setPressed(std::string name, bool pressed, float t) {
            std::string pressed_name = settings_.getNamePressed(name);
            std::string pressed_at_name = settings_.getNamePressedAt(name);
            std::string pressed_time_name = settings_.getNamePressedTime(name);

            // If pressed for the first time
            if (pressed) {
                bool previouslyPressed = params_.exists(pressed_name) ? params_.getBool(pressed_name) : false;
                if (!previouslyPressed) {
                    params_.set(settings::ParamSettings::floatValue(pressed_at_name, t));
                }

                params_.set(settings::ParamSettings::floatValue(pressed_time_name, t - params_.getFloat(pressed_at_name)));
            } else {
                params_.set(settings::ParamSettings::floatValue(pressed_time_name, 0));
            }

            params_.set(settings::ParamSettings::boolValue(pressed_name, pressed));
        }

        void Joystick::copyTo(param::Params& p) const {
            params_.copyTo(p);
        }
    }
}

#undef LOG_JOYSTICK
#undef AXIS_LOW
#undef AXIS_HIGH
