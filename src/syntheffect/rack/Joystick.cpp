#include "syntheffect/rack/Joystick.h"

#include "ofFileUtils.h"
#include "ofMath.h"

#include "yaml-cpp/yaml.h"
#include "GLFW/glfw3.h"

#include "syntheffect/rack/Channel.h"

#define LOG_JOYSTICK false

#include "ofLog.h"

#define AXIS_LOW -1
#define AXIS_HIGH 1
#define TRIGGER_LOW 0
#define TRIGGER_HIGH 1

namespace syntheffect {
    namespace rack {
        Joystick::Joystick(const std::string& id, const std::string& device_type) : Module(id), device_type_(device_type) {
        }

        void Joystick::connect(int glfw_id) {
            ofLogNotice("Joystick", "Connected joystick: %s", device_.c_str());
            glfw_id_ = glfw_id;
        }

        const std::string Joystick::getType() {
            return getModuleType();
        }

        const std::string Joystick::getModuleType() {
            return "core/joystick";
        }

        void Joystick::setup(int /* width */, int /* height */, int /* internal_format */, const std::string& /* modules_dir */) {
            std::string settings_path = ofFilePath::getAbsolutePath(ofFilePath::join("joysticks", device_type_ + ".yaml"));
            YAML::Node config = YAML::LoadFile(settings_path);

            device_ = config["device"].as<std::string>();
            deadzone_ = config["deadzone"].as<float>();

            for (YAML::const_iterator it=config["neutral"].begin(); it != config["neutral"].end(); ++it) {
                axis_neutrals_[it->first.as<int>()] = it->second.as<float>();
            }

            for (YAML::const_iterator it=config["stick_siblings"].begin(); it != config["stick_siblings"].end(); ++it) {
                stick_siblings_[it->first.as<int>()] = it->second.as<float>();
            }

            for (YAML::const_iterator it=config["axes"].begin(); it != config["axes"].end(); ++it) {
                std::string name = it->second.as<std::string>();
                axis_names_[it->first.as<int>()] = name;
                setupJoystickOutput(name);
            }

            for (YAML::const_iterator it=config["buttons"].begin(); it != config["buttons"].end(); ++it) {
                std::string name = it->second.as<std::string>();
                button_names_[it->first.as<int>()] = name;
                setupJoystickOutput(name);
            }

            for (YAML::const_iterator it=config["fake_buttons_negative"].begin(); it != config["fake_buttons_negative"].end(); ++it) {
                std::string name = it->first.as<std::string>();
                fake_buttons_negative_[name] = it->second.as<std::string>();
                setupJoystickOutput(name);
            }

            for (YAML::const_iterator it=config["fake_buttons_positive"].begin(); it != config["fake_buttons_positive"].end(); ++it) {
                std::string name = it->first.as<std::string>();
                fake_buttons_positive_[name] = it->second.as<std::string>();
                setupJoystickOutput(name);
            }

            for (const auto& node : config["triggers"]) {
                triggers_[node.as<std::string>()] = true;
            }
        }

        void Joystick::setupJoystickOutput(const std::string& name) {
            output_channels_[name + "_pressed"] = std::make_shared<Channel>(0);
            output_channels_[name + "_pressed_new"] = std::make_shared<Channel>(0);
            output_channels_[name] = std::make_shared<Channel>(0);
            output_channels_[name + "_pressed_time"] = std::make_shared<Channel>(0);
        }

        void Joystick::setJoystickOutput(const std::string& name, bool pressed, float t, float v) {
            if (pressed) {
                output_channels_[name + "_pressed"]->value_ = 1;
                output_channels_[name]->value_ = v;

                if (press_start_.count(name)) {
                    output_channels_[name + "_pressed_new"]->value_ = 0;
                } else {
                    output_channels_[name + "_pressed_new"]->value_ = 1;
                    press_start_[name] = t;
                }

                output_channels_[name + "_pressed_time"]->value_ = t - press_start_[name];
            } else {
                press_start_.erase(name);

                output_channels_[name + "_pressed"]->value_ = 0;
                output_channels_[name + "_pressed_new"]->value_ = 0;
                output_channels_[name]->value_ = 0;
                output_channels_[name + "_pressed_time"]->value_ = 0;
            }
        }

        void Joystick::update(float t) {
            if (glfwJoystickPresent(glfw_id_) != GLFW_TRUE) {
                return;
            }

            int axes_count;
            const float* axes = glfwGetJoystickAxes(glfw_id_, &axes_count);
            for (int i=0; i < axes_count; i++) {
                const std::string name = getAxisName(i);
                if (name == "") {
                    continue;
                }

                float v = axes[i];
                float deadzone = deadzone_;
                float neutral = getAxisNeutral(i);
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

                    // Remap to desired range
                    if (adjusted) {
                        if (triggers_.count(name)) {
                            v = ofMap(v, adjusted_low, adjusted_high, TRIGGER_LOW, TRIGGER_HIGH);
                        } else {
                            v = ofMap(v, adjusted_low, adjusted_high, AXIS_LOW, AXIS_HIGH);
                        }
                    }
                } else {
                    v = 0;
                }

                if (getInputConstant(name + "_abs", 0) > 0.5) {
                    v = std::abs(v);
                }

                setJoystickOutput(name, pressed, t, v);

#if LOG_JOYSTICK
                ofLogNotice("Joystick", "axis=%d name=%s raw=%f translated=%f adjusted=%i pressed=%i", i, name.c_str(), axes[i], v, adjusted, pressed);
#endif
            }

            int button_count;
            const unsigned char* buttons = glfwGetJoystickButtons(glfw_id_, &button_count);
            for (int i=0; i < button_count; i++) {
                const std::string name = getButtonName(i);
                if (name == "") {
                    continue;
                }

                bool pressed = buttons[i] == GLFW_PRESS;
                setJoystickOutput(name, pressed, t, pressed ? 1 : 0);

#if LOG_JOYSTICK
                ofLogNotice("Joystick", "button=%d name=%s pressed=%i", i, name.c_str(), pressed);
#endif
            }

            for (const auto& kv : fake_buttons_negative_) {
                std::string alias = kv.first;
                std::string axis_name = kv.second;

                if (!output_channels_.count(axis_name)) {
                    throw std::out_of_range("Was expecting module for joystick " + device_ + " to define output " + axis_name);
                }

                bool pressed = output_channels_.at(axis_name)->value_ < 0;
                setJoystickOutput(alias, pressed, t, pressed ? 1 : 0);
            }

            for (const auto& kv : fake_buttons_positive_) {
                std::string alias = kv.first;
                std::string axis_name = kv.second;

                if (!output_channels_.count(axis_name)) {
                    throw std::out_of_range("Was expecting module for joystick " + device_ + " to define output " + axis_name);
                }

                bool pressed = output_channels_.at(axis_name)->value_ > 0;
                setJoystickOutput(alias, pressed, t, pressed ? 1 : 0);
            }
        }

        bool Joystick::isCompatible(int glfw_id) {
            return std::string(glfwGetJoystickName(glfw_id)).find(device_) != std::string::npos;
        }

        bool Joystick::isAxisPressed(const float* axes, int i, int sibling) {
            float v = axes[i];
            float deadzone = deadzone_;
            float neutral = getAxisNeutral(i);

            bool pressed = (v - neutral < -deadzone) || (v - neutral > deadzone);
            if (sibling >= 0) {
                pressed |= isAxisPressed(axes, sibling);
            }

            return pressed;
        }

        int Joystick::getStickSibling(int i) {
            if (stick_siblings_.count(i) > 0) {
                return stick_siblings_.at(i);
            }

            return -1;
        }

        float Joystick::getAxisNeutral(int i) {
            if (axis_neutrals_.count(i) > 0) {
                return axis_neutrals_.at(i);
            }

            return 0;
        }

        std::string Joystick::getButtonName(int i) {
            if (!button_names_.count(i)) {
                return "";
            }

            return button_names_.at(i);
        }

        std::string Joystick::getAxisName(int i) {
            if (!axis_names_.count(i)) {
                return "";
            }

            return axis_names_.at(i);
        }
    }
}

#undef LOG_JOYSTICK
#undef AXIS_LOW
#undef AXIS_HIGH
#undef TRIGGER_LOW
#undef TRIGGER_HIGH
