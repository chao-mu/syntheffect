#pragma once

#include "ofShader.h"
#include "ofFbo.h"

#include "syntheffect/filter/FilterBase.h"
#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace filter {
        class ShaderFilterBase : public FilterBase {
            public:
                ShaderFilterBase(std::string name);
                void draw(graphics::PingPongBuffer& ping_pong, float t) override;

            protected:
                ofShader shader_;
                virtual void setupUniforms();
        };
    }
}