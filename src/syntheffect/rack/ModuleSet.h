#ifndef SYNTHEFFECT_RACK_MODULESET
#define SYNTHEFFECT_RACK_MODULESET

#include <memory>
#include <map>

#include "ofxSoundObject.h"

#include "yaml-cpp/yaml.h"

#include "syntheffect/rack/Module.h"
#include "syntheffect/rack/JoystickManager.h"

namespace syntheffect {
    namespace rack {
        class ModuleSet : public Module {
            public:
                ModuleSet(const std::string& id, const std::string& path, const std::string& modules_dir);
                void update(float t) override;
                bool updateUnready(float t);
                bool isReady() override;
                const std::string getType() override;
                void setup(int width, int height, int internal_format, ofxSoundOutput& sound_output, JoystickManager& joy_manager) override;
                void addModule(std::shared_ptr<Module> module);

            private:
                std::map<std::string, std::shared_ptr<Module>> modules_;
        };
    }
}

#endif
