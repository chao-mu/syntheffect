#pragma once

#include "syntheffect/param/Params.h"
#include "syntheffect/controller/Joystick.h"

namespace syntheffect {
    namespace controller {
        class XBoxController : public Joystick {
            public:
                XBoxController(int joystick_id);

                std::string getAxisName(int i) override;
                std::string getButtonName(int i) override;
                float getDeadzone() override;
        };
    }
}
