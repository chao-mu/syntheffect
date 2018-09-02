#ifndef SYNTHEFFECT_INPUT_CONTROLMAPPING_H
#define SYNTHEFFECT_INPUT_CONTROLMAPPING_H

#pragma once

#include <vector>

#include "syntheffect/param/Param.h"

namespace syntheffect {
    namespace input {
        struct ControlMapping {
            std::string control;

            // asset_id
            std::vector<std::string> first_press_activate_asset;
            // group name
            std::vector<std::string> first_press_activate_group;
            // stack name
            std::vector<std::string> first_press_shuffle_active_stack;
            // stack name
            std::vector<std::string> first_press_next_stack_asset;
            // stack name
            std::vector<std::string> first_press_prev_stack_asset;

            std::vector<param::Param> pressed_param;
            std::vector<param::Param> unpressed_param;
        };
    }
}

#endif
