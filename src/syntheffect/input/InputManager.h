#pragma once

#include <map>
#include <memory>
#include <string>

#include "ofEvent.h"

#include "syntheffect/input/JoystickSettings.h"
#include "syntheffect/input/Joystick.h"
#include "syntheffect/input/ControlMapping.h"
#include "syntheffect/param/Params.h"

namespace syntheffect {
    namespace input {
        struct ControlState {
            ControlMapping mapping;

            bool pressed = false;
            bool first_press = false;
            float pressed_time = 0;
        };

        class InputManager {
            public:
                JoystickID addJoystick(JoystickSettings js);
                void addControlMapping(JoystickID joy_id, ControlMapping mapping);

                void setup();

                void update(float t);

                ofFastEvent<const ControlState> state_events;

            private:
                void assignJoysticks();
                int nextID();

                JoystickID last_id_ = 0;
                std::map<JoystickID, int> glfw_ids_;
                std::map<JoystickID, std::shared_ptr<Joystick>> joysticks_;
                std::map<JoystickID, std::vector<ControlMapping>> mappings_;
        };
    }
}
