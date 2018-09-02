#ifndef SYNTHEFFECT_INPUT_JOYSTICKSETTINGS_H
#define SYNTHEFFECT_INPUT_JOYSTICKSETTINGS_H

#pragma once

#include <string>
#include <map>

namespace syntheffect {
    namespace input {
        using JoystickID = int;

        struct JoystickSettings {
            JoystickID id;
            std::string device;
            std::map<int, float> axis_neutrals;
            std::map<int, std::string> axis_names;
            std::map<int, std::string> button_names;
            std::map<int, int> stick_siblings;
            std::map<std::string, std::string> fake_buttons_negative;
            std::map<std::string, std::string> fake_buttons_positive;
            float deadzone;

            float getAxisNeutral(int i);
            std::string getButtonName(int i);
            std::string getAxisName(int i);
        };
    }
}

#endif
