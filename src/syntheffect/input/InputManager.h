#pragma once

#include <map>
#include <memory>
#include <string>

#include "ofEvent.h"

#include "syntheffect/input/JoystickSettings.h"
#include "syntheffect/input/Joystick.h"
#include "syntheffect/param/Params.h"
#include "syntheffect/settings/ParamSettings.h"

namespace syntheffect {
    namespace input {
        class InputManager {
            public:
                JoystickID addJoystick(JoystickSettings js);
                void addTriggerAssetGroup(JoystickID joy_id, std::string name, std::string control);
                void addTriggerParamSet(JoystickID joy_id, std::string control, settings::ParamSettings param);

                void setup();

                void update(float t);

                ofEvent<std::string> asset_trigger_events;
                ofEvent<const settings::ParamSettings> param_trigger_events;

            private:
                void assignJoysticks();
                int nextID();

                JoystickID last_id_ = 0;
                std::map<JoystickID, int> glfw_ids_;
                std::map<JoystickID, std::shared_ptr<Joystick>> joysticks_;
                std::map<JoystickID, std::map<std::string, std::string>> asset_triggers_;
                std::map<JoystickID, std::map<std::string, std::vector<settings::ParamSettings>>> param_triggers_;
        };
    }
}
