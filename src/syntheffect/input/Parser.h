#pragma once

#include "ofXml.h"

#include "syntheffect/input/InputManager.h"

namespace syntheffect {
    namespace input {
        class Parser {
            public:
                static void addInputs(InputManager& manager, std::string path);
                static void addJoystick(InputManager& manager, const ofXml& xml);
        };
    }
}
