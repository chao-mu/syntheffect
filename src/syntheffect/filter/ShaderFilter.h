#pragma once

#include "ofShader.h"
#include "ofFbo.h"

#include "syntheffect/filter/FilterBase.h"
#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace filter {
        class ShaderFilter : public FilterBase {
            public:
                ShaderFilter(std::string name);
                void draw(graphics::PingPongBuffer& ping_pong, float t) override;

            protected:
                ofShader shader_;
                virtual void setupUniforms();
        };
    }
}