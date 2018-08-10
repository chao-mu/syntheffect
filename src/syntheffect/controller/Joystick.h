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
                virtual float getAxisNeutral(int i) = 0;
                virtual std::string getButtonName(int i) = 0;
                virtual int getDeadzoneSibling(int i) = 0;

                std::string getNamePressedAt(std::string name);
                std::string getNamePressedTime(std::string name);
                std::string getNamePressed(std::string name);

                void setPressed(std::string name, bool pressed, float t);

                bool isAxisPressed(const float* axes, int i, int sibling=-1);

                virtual float getDeadzone();
        };
    }
}
