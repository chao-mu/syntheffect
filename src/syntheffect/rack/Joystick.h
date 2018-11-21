#ifndef SYNTHEFFECT_RACK_JOYSTICK
#define SYNTHEFFECT_RACK_JOYSTICK

#include <string>

#include "syntheffect/rack/Module.h"

namespace syntheffect {
    namespace rack {
        class Joystick : public Module {
            public:
                Joystick(const std::string& id, const std::string& path);

                void setup(int width, int height, int internal_format, const std::string& workspace_dir) override;
                void update(float t) override;

                bool isCompatible(int glfw_id);
                void connect(int glfw_id);

                virtual const std::string getType() override;

                static const std::string getModuleType();

            private:
                void setupJoystickOutput(const std::string& name);
                void setJoystickOutput(const std::string& name, bool pressed, float t, float v);

                bool isAxisPressed(const float* axes, int i, int sibling=-1);
                int getStickSibling(int i);

                std::map<std::string, float> press_start_;
                int glfw_id_;

                std::string device_;
                const std::string path_;
                std::map<int, float> axis_neutrals_;
                std::map<int, std::string> axis_names_;
                std::map<int, std::string> button_names_;
                std::map<int, int> stick_siblings_;
                std::map<std::string, std::string> fake_buttons_negative_;
                std::map<std::string, std::string> fake_buttons_positive_;
                float deadzone_;

                float getAxisNeutral(int i);
                std::string getButtonName(int i);
                std::string getAxisName(int i);


                std::map<std::string, bool> triggers_;
        };
    }
}

#endif
