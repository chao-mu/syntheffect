#pragma once

#include "syntheffect/param/Params.h"

namespace syntheffect {
    namespace controller {
        class Joystick {
            public:
                Joystick(int joystick_id);

                bool isPresent();

                void update();

                std::shared_ptr<param::Params> getParams();

                std::string getName();

            private:
                int id_;
                std::shared_ptr<param::Params> params_;

                virtual std::string getAxisName(int i) = 0;
                virtual std::string getButtonName(int i) = 0;
                std::string getButtonNameLastPressed(int i);

                virtual float getDeadzone();
        };
    }
}
