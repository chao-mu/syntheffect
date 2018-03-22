#pragma once

#include "syntheffect/filter/ShaderFilterBase.h"
#include "syntheffect/filter/HasIntensityBase.h"

namespace syntheffect {
    namespace filter {
        class Delay : public ShaderFilterBase, public HasIntensityBase {
            public:
                Delay(bool active=false);
                void setLastTexture(ofTexture last);

            protected:    
                void setupUniforms();
                ofTexture last_texture_;
        };
    }
}