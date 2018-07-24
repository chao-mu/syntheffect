#include "syntheffect/controller/XBoxController.h"

namespace syntheffect {
    namespace controller {
        XBoxController::XBoxController(int joystick_id) : Joystick(joystick_id) {

        }

        float XBoxController::getDeadzone() {
            return 0.15f;
        }

        std::string XBoxController::getAxisName(int i) {
            switch (i) {
                case 0: return "xbox_left_stick_x";
                case 1: return "xbox_left_stick_y";
                case 3: return "xbox_right_stick_x";
                case 4: return "xbox_right_stick_y";
                case 6: return "xbox_dir_pad_x";
                case 7: return "xbox_dir_pad_y";
                case 2: return "xbox_left_trigger";
                case 5: return "xbox_right_trigger";
                default: return "";
            }
        }

        std::string XBoxController::getButtonName(int i) {
            switch (i) {
                case 2: return "xbox_button_x";
                case 3: return "xbox_button_y";
                case 1: return "xbox_button_b";
                case 0: return "xbox_button_a";
                case 4: return "xbox_button_top_left";
                case 5: return "xbox_button_top_right";
                case 6: return "xbox_button_middle_left";
                case 7: return "xbox_button_middle_right";
                default: return "";
            }
        }
    }
}
