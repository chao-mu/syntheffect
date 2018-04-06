#include "syntheffect/effect/Shader.h"

#include "ofGraphics.h"

#include "ofxYAML.h"

namespace syntheffect {
    namespace effect {
        Shader::Shader() : Effect(), shader_() {
        }

        bool Shader::load(std::string frag, std::string vert) {
            return shader_.load(
                "shadersGL3/shaders/vert/" + vert + ".vert",
                "shadersGL3/shaders/frag/" + frag + ".frag"
            );
        }


        void Shader::set(std::string name, std::function<void(std::string, ofShader&)> setter) {
            uniform_setters_[name] = setter;
        }

        void Shader::draw(graphics::PingPongBuffer& ping_pong, float t) {
            if (!isActive()) {
                return;
            }

            ping_pong.dest->begin();
            ofClear(0, 255);

            ofTexture tex = ping_pong.src->getTexture();
            shader_.begin();
            
            shader_.setUniform2f("resolution", tex.getWidth(), tex.getHeight());
            shader_.setUniform1f("time", t);
            for (const auto& kv : uniform_setters_) {
                kv.second(kv.first, shader_);
            }

            tex.draw(0, 0);

            shader_.end();
            ping_pong.dest->end();
        }
    }
}

#undef DEFAULT_FRAG
#undef DEFAULT_VERT