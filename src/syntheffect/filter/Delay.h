#pragma once

#include "syntheffect/filter/ShaderFilterBase.h"
#include "syntheffect/filter/HasIntensityBase.h"

namespace syntheffect {
    namespace filter {
        class Delay : public ShaderFilterBase, public HasIntensityBase {
            public:
                Delay();
                void setLastTexture(ofTexture last);

            protected:    
                void setupUniforms() override;
                ofTexture last_texture_;
        };
    }
}