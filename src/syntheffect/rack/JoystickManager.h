#ifndef SYNTHEFFECT_RACK_JOYSTICKMANAGER
#define SYNTHEFFECT_RACK_JOYSTICKMANAGER

#include <memory>

#include "syntheffect/rack/Joystick.h"

namespace syntheffect {
    namespace rack {
        class JoystickManager {
            using JoystickID = int;
            public:
                void addJoystick(std::shared_ptr<Joystick> joystick);
                void update(float t);

            private:
                JoystickID next_id_ = 0;
                std::map<JoystickID, int> glfw_ids_;
                std::map<JoystickID, std::shared_ptr<Joystick>> joysticks_;
        };
    }
}
#endif
