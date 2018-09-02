#include "syntheffect/input/JoystickSettings.h"

#include <stdexcept>

namespace syntheffect {
    namespace input {
        float JoystickSettings::getAxisNeutral(int i) {
            if (axis_neutrals.count(i) > 0) {
                return axis_neutrals.at(i);
            }

            return 0;
        }

        std::string JoystickSettings::getButtonName(int i) {
            if (!button_names.count(i)) {
                throw std::out_of_range("Button name not configured for button " + std::to_string(i));
            }

            return button_names.at(i);
        }

        std::string JoystickSettings::getAxisName(int i) {
            if (!axis_names.count(i)) {
                throw std::out_of_range("Axis name not configured for axis " + std::to_string(i));
            }

            return axis_names.at(i);
        }
    }
}
