#pragma once

#include "ofShader.h"
#include "ofTexture.h"

#include "syntheffect/filter/FilterBase.h"

namespace syntheffect {
    namespace filter {
        class ShaderFilterBase : public FilterBase {
            public:
                ShaderFilterBase(std::string name);
                void end() override;
                void begin(float width, float height, float t) override;

            protected:
                ofShader shader_;
                virtual void setupUniforms();
        };
    }
}