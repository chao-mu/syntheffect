#pragma once

#include <map>
#include <memory>
#include <string>

#include "ofEvent.h"

#include "syntheffect/input/JoystickSettings.h"
#include "syntheffect/input/Joystick.h"
#include "syntheffect/param/Params.h"
#include "syntheffect/param/Param.h"

namespace syntheffect {
    namespace input {
        class InputManager {
            public:
                JoystickID addJoystick(JoystickSettings js);
                void addSetAssetGroup(JoystickID joy_id, std::string contol, std::string target);
                void addSetParam(JoystickID joy_id, std::string control, param::Param param);

                void setup();

                void update(float t);

                ofEvent<std::string> asset_trigger_events;
                ofEvent<const param::Param> param_trigger_events;

            private:
                void assignJoysticks();
                int nextID();

                JoystickID last_id_ = 0;
                std::map<JoystickID, int> glfw_ids_;
                std::map<JoystickID, std::shared_ptr<Joystick>> joysticks_;
                std::map<JoystickID, std::map<std::string, std::string>> asset_triggers_;
                std::map<JoystickID, std::map<std::string, std::vector<param::Param>>> param_triggers_;
        };
    }
}
