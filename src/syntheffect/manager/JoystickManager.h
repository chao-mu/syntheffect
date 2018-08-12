#pragma once

#include <map>

#include "syntheffect/settings/JoystickSettings.h"
#include "syntheffect/controller/Joystick.h"
#include "syntheffect/param/Params.h"

namespace syntheffect {
    namespace manager {
        typedef int JoystickID;

        class JoystickManager {
            public:
                void addJoystick(settings::JoystickSettings js);

                void setup();

                void update(float t);

                void copyTo(param::Params& params);


            private:
                void assignJoysticks();
                int nextID();

                JoystickID last_id_ = 0;
                std::map<JoystickID, int> glfw_ids_;
                std::map<JoystickID, std::shared_ptr<controller::Joystick>> joysticks_;
        };
    }
}
