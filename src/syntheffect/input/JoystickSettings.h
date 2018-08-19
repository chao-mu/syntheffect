#pragma once

#include <string>
#include <map>

namespace syntheffect {
    namespace input {
        typedef int JoystickID;

        struct JoystickSettings {
            JoystickID id;
            std::string device;
            std::string prefix;
            std::map<int, float> axis_neutrals;
            std::map<int, std::string> axis_names;
            std::map<int, std::string> button_names;
            std::map<int, int> stick_siblings;
            float deadzone;

            float getAxisNeutral(int);

            std::string withPrefix(std::string name);

            std::string getButtonName(int);
            std::string getButtonNamePressed(int);
            std::string getButtonNamePressedTime(int);
            std::string getButtonNamePressedAt(int);

            std::string getAxisName(int);
            std::string getAxisNamePressed(int);
            std::string getAxisNamePressedTime(int);
            std::string getAxisNamePressedAt(int);

            std::string getNamePressed(std::string name);
            std::string getNamePressedTime(std::string name);
            std::string getNamePressedAt(std::string name);
        };
    }
}
