#pragma once

#include "ofShader.h"
#include "ofFbo.h"

#include "syntheffect/filter/FilterBase.h"
#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/filter/Param.h"

namespace syntheffect {
    namespace filter {
        class ShaderFilter : public FilterBase {
            public:
                ShaderFilter();
                void draw(graphics::PingPongBuffer& ping_pong, float t) override;
                bool load(std::string path);

            private:
                ofShader shader_;
                vector<shared_ptr<ParamBase>> params_;
        };
    }
}