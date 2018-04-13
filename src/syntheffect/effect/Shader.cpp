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

            int tex_count = 1;
            for (const auto& kv : texture_params_) {
                shader_.setUniformTexture(kv.first, kv.second(), tex_count++);
            }
        }

        void Shader::draw(shared_ptr<graphics::PingPongBuffer> ping_pong, float t) {
            if (!isActive()) {
                return;
            }

            shader_.begin();
            ping_pong->begin();
            ofClear(0, 255);
            
            shader_.setUniform2f("resolution", ping_pong->getWidth(), ping_pong->getHeight());
            shader_.setUniform1f("time", t);

            setUniforms();

            ping_pong->drawable()->draw(0, 0);

            ping_pong->end();
            shader_.end();
        }
    }
}

#undef DEFAULT_FRAG
#undef DEFAULT_VERT