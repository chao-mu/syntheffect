#ifndef SYNTHEFFECT_RACK_SHADER
#define SYNTHEFFECT_RACK_SHADER

#include <string>
#include <vector>
#include <unordered_set>

#include "ofShader.h"
#include "ofFbo.h"
#include "ofMesh.h"

#include "syntheffect/rack/Module.h"

namespace syntheffect {
    namespace rack {
        class Shader : public Module {
            public:
                Shader(const std::string& id, const std::string& path);

                virtual void setup(int width, int height, int internal_format, const std::string& workspace_dir) override;
                virtual void update(float t) override;
                virtual bool isReady() override;
                virtual const std::string getType() override;
                virtual bool inputExists(const std::string& name) override;

                static const std::string getModuleType();
            
            void registerInputs(std::vector<std::string> inputs);

            private:
                ofShader shader_;
                ofMesh mesh_;

                ofFbo outputs_;
                std::vector<ofFbo> last_outputs_;
                std::unordered_set<std::string> input_names_;

                const std::string path_;
                bool first_pass_;
        };
    }
}

#endif
