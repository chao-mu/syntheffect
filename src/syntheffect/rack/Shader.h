#ifndef SYNTHEFFECT_RACK_SHADER
#define SYNTHEFFECT_RACK_SHADER
#include <string>
#include <vector>

#include "ofShader.h"
#include "ofFbo.h"
#include "ofMesh.h"

#include "syntheffect/rack/Module.h"

namespace syntheffect {
    namespace rack {
        class Shader : public Module {
            public:
                Shader(const std::string& id, const std::string& path);

                virtual void setup(int width, int height, int internal_format) override;
                virtual void update(float t) override;
                virtual bool isReady() override;

            private:
                ofShader shader_;
                ofMesh mesh_;

                ofFbo outputs_;
                std::vector<ofFbo> last_outputs_;
                std::vector<std::string> input_names_;

                const std::string path_;
                bool first_pass_;
        };
    }
}

#endif
