#include "syntheffect/controller/Joystick.h"

#include "GLFW/glfw3.h"

#include "ofLog.h"
#include "ofUtils.h"

#define LOG_JOYSTICK_AXES false
#define LOG_JOYSTICK_BUTTONS false

namespace syntheffect {
    namespace controller {
        Joystick::Joystick(int joystick_id) : params_(std::make_shared<param::Params>()) {
            id_ = joystick_id;
        }

        bool Joystick::isPresent() {
            return glfwJoystickPresent(id_) == GLFW_TRUE;
        }

        void Joystick::update() {
            if (isPresent()) {
                int axes_count;
                const float* axes = glfwGetJoystickAxes(id_, &axes_count);
                for (int i=0; i < axes_count; i++) {
                    std::string name = getAxisName(i);
                    float v = axes[i];
                    if (v < -getDeadzone() || v > getDeadzone()) {
                        params_->float_params[name] = v;
                    } else {
                        params_->float_params[name] = 0;
                    }

                    if (LOG_JOYSTICK_AXES) {
                        ofLog() << name << " " << axes[i];
                    }
                }

                int button_count;
                const unsigned char* buttons = glfwGetJoystickButtons(id_, &button_count);
                for (int i=0; i < button_count; i++) {
                    std::string name = getButtonName(i);

                    bool pressed = buttons[i] == GLFW_PRESS;
                    // If pressed for the first time
                    bool previouslyPressed = params_->bool_params[name];
                    if (pressed && !previouslyPressed) {
                        params_->float_params[getButtonNameLastPressed(i)] = ofGetElapsedTimef();
                    }

                    params_->bool_params[name] = pressed;

                    if (LOG_JOYSTICK_BUTTONS) {
                        ofLog() << name << " " << pressed << " " << params_->float_params[getButtonNameLastPressed(i)];
                    }
                }
            }
        }

        std::string Joystick::getName() {
            return glfwGetJoystickName(id_);
        }

        std::shared_ptr<param::Params> Joystick::getParams() {
            return params_;
        }

        std::string Joystick::getButtonNameLastPressed(int i) {
            return getButtonName(i) + "_pressed_at";
        }

        float Joystick::getDeadzone() {
            return 0;
        }
    }
}

#undef LOG_JOYSTICK_AXES
#undef LOG_JOYSTICK_BUTTONS
