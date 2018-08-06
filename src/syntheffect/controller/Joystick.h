#pragma once

#include "syntheffect/param/Params.h"

namespace syntheffect {
    namespace controller {
        class Joystick {
            public:
                Joystick(int joystick_id);

                bool isPresent();

                void update(float t);

                void copyTo(param::Params& p);

                std::string getName();

            private:
                int id_;
                param::Params params_;

                virtual std::string getAxisName(int i) = 0;
                virtual std::string getButtonName(int i) = 0;
                std::string getButtonNameLastPressed(int i);
                std::string getAxisNameLastPressed(int i);

                virtual float getDeadzone();
        };
    }
}
