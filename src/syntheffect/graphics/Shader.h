#ifndef SYNTHEFFECT_GRAPHICS_SHADER_H
#define SYNTHEFFECT_GRAPHICS_SHADER_H

#pragma once

#include "ofShader.h"
#include "ofFbo.h"
#include "ofXml.h"

#include "syntheffect/graphics/Effect.h"
#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace graphics {
        class Shader : public Effect {
            public:
                Shader();
                void drawTo(std::shared_ptr<graphics::PingPongBuffer> ping_pong) override;
                void load(const std::string& frag, const std::string& vert);
                void setUniforms();

            private:
                ofShader shader_;
        };
    }
}

#endif
