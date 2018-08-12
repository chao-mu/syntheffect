#pragma once

#include "syntheffect/param/Params.h"
#include "syntheffect/settings/JoystickSettings.h"

namespace syntheffect {
    namespace controller {
        class Joystick {
            public:
                Joystick(settings::JoystickSettings settings);

                void update(float t, int id);

                void copyTo(param::Params& p) const;

                bool isCompatible(int glfw_id);

            private:
                param::Params params_;
                settings::JoystickSettings settings_;

                void setPressed(std::string name, bool pressed, float t);

                bool isAxisPressed(const float* axes, int i, int sibling=-1);

                int getStickSibling(int i);
        };
    }
}
