#ifndef SYNTHEFFECT_RACK_RACK
#define SYNTHEFFECT_RACK_RACK

#include <map>
#include <string>

#include "ofTexture.h"
#include "ofFbo.h"

#include "ofxSoundObject.h"

#include "yaml-cpp/yaml.h"

#include "syntheffect/rack/ModuleSet.h"
#include "syntheffect/rack/JoystickManager.h"

namespace syntheffect {
    namespace rack {
        class Rack {
            public:
                Rack(const std::string& path, const std::string& modules_dir);

                void setup(size_t audio_buffer_size, int internal_format=GL_RGBA32F);
                void start();
                void update(float t);
                bool updateUnready(float t);

                ofTexture& getTexture();

                ofBaseSoundOutput& getSoundOutput();

                int getWidth();
                int getHeight();

            private:
                ofFbo fbo_;

                bool is_ready_;

                JoystickManager joy_manager_;
                std::shared_ptr<ModuleSet> modules_;

                ofxSoundOutput sound_output_;
        };
    }
}
#endif
