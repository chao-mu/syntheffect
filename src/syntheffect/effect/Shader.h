#pragma once

#include "ofShader.h"
#include "ofFbo.h"
#include "ofXml.h"

#include "syntheffect/effect/Effect.h"
#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace effect {
        class Shader : public Effect {
            public:
                Shader();
                void draw(shared_ptr<graphics::PingPongBuffer> ping_pong, float t) override;
                bool load(std::string frag, std::string vert);
                void setUniforms();

            private:
                ofShader shader_;
        };
    }
}