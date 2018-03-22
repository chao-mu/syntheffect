
#include "syntheffect/filter/ShaderFilterBase.h"

namespace syntheffect {
    namespace filter {
        ShaderFilterBase::ShaderFilterBase(bool active, std::string name) : FilterBase(active) {
            shader_ = ofShader();
            shader_.load("shadersGL3/passthrough.vert", "shadersGL3/" + name + ".frag");;
        }
        
        void ShaderFilterBase::setupUniforms() {
        
        }

        void ShaderFilterBase::begin(float width, float height, float t) {
            if (isActive()) {
                shader_.begin();
                shader_.setUniform2f("resolution", width, height);
                shader_.setUniform1f("time", t);
                setupUniforms();
            }
        }

        void ShaderFilterBase::end() {
            if (isActive()) {
                shader_.end();
            }
        }

        void ShaderFilterBase::loadFragShader(std::string name) {
            
        }
    } 
}