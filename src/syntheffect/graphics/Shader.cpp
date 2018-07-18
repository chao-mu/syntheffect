#include "syntheffect/graphics/Shader.h"

#include "ofGraphics.h"

namespace syntheffect {
    namespace graphics {
        Shader::Shader() : Effect(), shader_() {
        }

        bool Shader::load(std::string frag, std::string vert) {
            return shader_.load(
                "shaders/vert/" + vert + ".vert",
                "shaders/frag/" + frag + ".frag"
            );
        }

        void Shader::setUniforms() {
            for (const auto& kv : params.float_params) {
                shader_.setUniform1f(kv.first, kv.second);
            }

            for (const auto& kv : params.int_params) {
                shader_.setUniform1i(kv.first, kv.second);
            }

            for (const auto& kv : params.bool_params) {
                shader_.setUniform1i(kv.first, kv.second ? 1 : 0);
            }

            int tex_count = 1;
            for (const auto& kv : params.float_func_params) {
                shader_.setUniform1f(kv.first, kv.second());
            }

            for (const auto& kv : params.int_func_params) {
                shader_.setUniform1i(kv.first, kv.second());
            }

            for (const auto& kv : params.bool_func_params) {
                shader_.setUniform1i(kv.first, kv.second() ? 1 : 0);
            }

            for (const auto& kv : params.texture_func_params) {
                shader_.setUniformTexture(kv.first, kv.second(), tex_count++);
            }
        }

        void Shader::drawTo(std::shared_ptr<graphics::PingPongBuffer> ping_pong) {
            if (!isActive()) {
                return;
            }

            shader_.begin();
            ping_pong->begin();
            ofClear(0);

            shader_.setUniform2f("resolution", ping_pong->getWidth(), ping_pong->getHeight());

            setUniforms();

            ping_pong->drawable()->draw(0, 0);

            ping_pong->end();
            shader_.end();
        }
    }
}

#undef DEFAULT_FRAG
#undef DEFAULT_VERT
