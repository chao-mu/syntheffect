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


        void Shader::setUniforms() {
            for (const auto& kv : float_params_) {
                shader_.setUniform1f(kv.first, kv.second());
            }

            for (const auto& kv : int_params_) {
                shader_.setUniform1i(kv.first, kv.second());
            }

            for (const auto& kv : bool_params_) {
                shader_.setUniform1i(kv.first, kv.second() ? 1 : 0);
            }
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

            setUniforms();

            tex.draw(0, 0);

            shader_.end();
            ping_pong.dest->end();
        }
    }
}

#undef DEFAULT_FRAG
#undef DEFAULT_VERT