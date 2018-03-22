#include "syntheffect/filter/Delay.h"

namespace syntheffect {
    namespace filter {
        Delay::Delay() : ShaderFilterBase(std::string("Delay")) {
        }

        void Delay::setupUniforms() {
            
            float intensity = getIntensity(0, 1);
            float weight = 1.0 - (1.0 / (1.0 + (intensity * 15.0)));
            shader_.setUniform1f("weight", weight);
            shader_.setUniformTexture("last", last_texture_, 1);
        }

        void Delay::setLastTexture(ofTexture last) {
            last_texture_ = last;
        }
    }
}