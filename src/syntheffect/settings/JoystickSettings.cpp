#include "syntheffect/settings/JoystickSettings.h"

namespace syntheffect {
    namespace settings {
        float JoystickSettings::getAxisNeutral(int i) {
            if (axis_neutrals.count(i) > 0) {
                return axis_neutrals.at(i);
            }

            return 0;
        }

        std::string JoystickSettings::withPrefix(std::string name)  {
            if (prefix != "") {
                name += "-" + prefix;
            }

            return name;
        }

        std::string JoystickSettings::getButtonName(int i) {
            if (button_names.count(i) > 0) {
                return withPrefix(button_names.at(i));
            }

            return "";
        }


        std::string JoystickSettings::getButtonNamePressed(int i) {
            return getNamePressed(getButtonName(i));
        }

        std::string JoystickSettings::getButtonNamePressedAt(int i) {
            return getNamePressedAt(getButtonName(i));
        }


        std::string JoystickSettings::getButtonNamePressedTime(int i) {
            return getNamePressedTime(getButtonName(i));
        }


        std::string JoystickSettings::getAxisName(int i) {
            if (axis_names.count(i) > 0) {
                return withPrefix(axis_names.at(i));
            }

            return "";
        }


        std::string JoystickSettings::getAxisNamePressed(int i) {
            return getNamePressed(getAxisName(i));
        }


        std::string JoystickSettings::getAxisNamePressedAt(int i) {
            return getNamePressedAt(getAxisName(i));
        }


        std::string JoystickSettings::getAxisNamePressedTime(int i) {
            return getNamePressedTime(getAxisName(i));
        }


        std::string JoystickSettings::getNamePressedAt(std::string name) {
            return name + "-pressed_at";
        }

        std::string JoystickSettings::getNamePressedTime(std::string name) {
            return name + "-pressed_time";
        }

        std::string JoystickSettings::getNamePressed(std::string name) {
            return name + "-pressed";
        }
    }
}
