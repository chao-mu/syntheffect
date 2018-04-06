#pragma once

#include "ofShader.h"
#include "ofFbo.h"

#include "syntheffect/effect/Effect.h"
#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace effect {
        class Shader : public Effect {
            public:
                Shader();
                void draw(graphics::PingPongBuffer& ping_pong, float t) override;
                bool load(std::string path);
                void set(std::string name, std::function<void(std::string, ofShader&)> setter);

            private:
                ofShader shader_;
                map<std::string, std::function<void(std::string, ofShader&)>> uniform_setters_;
        };
    }
}