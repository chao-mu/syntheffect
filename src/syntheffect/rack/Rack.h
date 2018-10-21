#ifndef SYNTHEFFECT_RACK_RACK
#define SYNTHEFFECT_RACK_RACK

#include <map>
#include <string>

#include "ofTexture.h"
#include "ofFbo.h"

#include "ofxSoundObject.h"

#include "yaml-cpp/yaml.h"

#include "syntheffect/rack/Module.h"
#include "syntheffect/rack/JoystickManager.h"

namespace syntheffect {
    namespace rack {
        class Rack {
            public:
                Rack(const std::string& path);

                void setup(size_t audio_buffer_size, int internal_format=GL_RGBA);
                void start();
                void update(float t);
                bool updateUnready(float t);

                ofTexture& getTexture();

                void addModule(std::shared_ptr<Module> module);

                ofBaseSoundOutput& getSoundOutput();

                int getWidth();
                int getHeight();

            private:
                std::map<std::string, std::shared_ptr<Module>> modules_;
                ofFbo fbo_;

                std::string path_;

                bool is_ready_;

                JoystickManager joy_manager_;

                ofxSoundOutput sound_output_;

        };
    }
}
#endif
