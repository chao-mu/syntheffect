#include "syntheffect/controller/Joystick.h"

#include "GLFW/glfw3.h"

#include "ofLog.h"
#include "ofUtils.h"
#include "ofMath.h"

#define LOG_JOYSTICK false

namespace syntheffect {
    namespace controller {
        Joystick::Joystick(int joystick_id) {
            id_ = joystick_id;
        }

        bool Joystick::isPresent() {
            return glfwJoystickPresent(id_) == GLFW_TRUE;
        }

        void Joystick::update(float t) {
            if (isPresent()) {
                int axes_count;
                const float* axes = glfwGetJoystickAxes(id_, &axes_count);
                for (int i=0; i < axes_count; i++) {
                    std::string name = getAxisName(i);
                    if (name == "") {
                        continue;
                    }

                    float v = axes[i];
                    float deadzone = getDeadzone();
                    float last_press = ofGetElapsedTimef();
                    if (v < -deadzone) {
                        v += deadzone;
                    } else if (v > deadzone) {
                        v -= deadzone;
                    } else {
                        v = 0;
                        last_press = 0;
                    }

                    // Remap to -1 to 1
                    v = ofMap(v, -1 + deadzone, 1 - deadzone, -1, 1);

                    params_.set(name, v);
                    params_.set(getAxisNameLastPressed(i), last_press);

                    if (LOG_JOYSTICK) {
                        ofLogNotice("Joystick", "axis=%d name=%s raw=%f processed=%f", i, name.c_str(), axes[i], v);
                    }
                }

                int button_count;
                const unsigned char* buttons = glfwGetJoystickButtons(id_, &button_count);
                for (int i=0; i < button_count; i++) {
                    std::string name = getButtonName(i);
                    if (name == "") {
                        continue;
                    }

                    bool pressed = buttons[i] == GLFW_PRESS;
                    // If pressed for the first time
                    bool previouslyPressed = params_.getBool(name, false);
                    if (pressed && !previouslyPressed) {
                        params_.set(getButtonNameLastPressed(i), ofGetElapsedTimef());
                    }

                    params_.set(name, pressed);

                    if (LOG_JOYSTICK) {
                        ofLogNotice("Joystick", "button=%d name=%s pressed=%i previous=%i", i, name.c_str(), pressed, previouslyPressed);
                    }
                }
            }
        }

        std::string Joystick::getName() {
            return glfwGetJoystickName(id_);
        }

        void Joystick::copyTo(param::Params& p) {
            params_.copyTo(p);
        }

        std::string Joystick::getButtonNameLastPressed(int i) {
            return getButtonName(i) + "_pressed_at";
        }

        std::string Joystick::getAxisNameLastPressed(int i) {
            return getAxisName(i) + "_pressed_at";
        }

        float Joystick::getDeadzone() {
            return 0;
        }
    }
}

#undef LOG_JOYSTICK
