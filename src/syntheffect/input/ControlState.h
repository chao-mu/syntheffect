#ifndef SYNTHEFFECT_INPUT_CONTROLSTATE_H
#define SYNTHEFFECT_INPUT_CONTROLSTATE_H

#pragma once

#include <map>
#include <memory>
#include <string>

#include "ofEvent.h"

namespace syntheffect {
    namespace input {
        struct ControlState {
            ControlMapping mapping;

            bool pressed = false;
            bool first_press = false;
            float pressed_time = 0;
        };
    }
}

#endif
