#pragma once

#include "syntheffect/graphics/Params.h"

namespace syntheffect {
    namespace controller {
        class Joystick {
            public:
                Joystick(int joystick_id);

                bool isPresent();

                void update();

                std::shared_ptr<graphics::Params> getParams();

                std::string getName();

            private:
                int id_;
                std::shared_ptr<graphics::Params> params_;

                std::string getAxisName(int i);
                std::string getButtonName(int i);
                std::string getButtonNameLastPressed(int i);
        };
    }
}
