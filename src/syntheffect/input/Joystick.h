#ifndef SYNTHEFFECT_INPUT_JOYSTICK_H
#define SYNTHEFFECT_INPUT_JOYSTICK_H

#pragma once

#include "ofEvent.h"

#include "syntheffect/param/Params.h"
#include "syntheffect/input/JoystickSettings.h"

namespace syntheffect {
    namespace input {
        struct JoystickPress {
            std::string name;
            float pressed_time;
            bool first;
            float value;
        };

        class Joystick {
            public:
                Joystick(JoystickSettings settings);

                void addListener(std::function<void(JoystickPress&)> f);
                std::map<std::string, JoystickPress> getPresses(float t, int id);

                bool isCompatible(int glfw_id);

            private:
                std::map<std::string, float> press_start_;
                JoystickSettings settings_;

                JoystickPress makePress(std::string name, float t, float value=0);

                bool isAxisPressed(const float* axes, int i, int sibling=-1);

                int getStickSibling(int i);
        };
    }
}

#endif
