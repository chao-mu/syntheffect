#pragma once

#include "ofShader.h"
#include "ofTexture.h"

#include "syntheffect/filter/FilterBase.h"

namespace syntheffect {
    namespace filter {
        class ShaderFilterBase : public FilterBase {
            public:
                ShaderFilterBase(bool active, std::string name);
                void end();
                void begin(float width, float height, float t);

            protected:
                ofShader shader_;
                virtual void setupUniforms();
                void loadFragShader(std::string name);
        };
    }
}