#pragma once

#include "syntheffect/filter/ShaderFilter.h"
#include "syntheffect/filter/HasIntensityBase.h"

namespace syntheffect {
    namespace filter {
        class Delay : public ShaderFilter, public HasIntensityBase {
            public:
                Delay();
                void setLastTexture(ofTexture last);

            protected:    
                void setupUniforms() override;
                ofTexture last_texture_;
        };
    }
}