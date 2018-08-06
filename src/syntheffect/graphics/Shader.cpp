#include "syntheffect/graphics/Shader.h"

#include "ofGraphics.h"

namespace syntheffect {
    namespace graphics {
        Shader::Shader() : Effect(), shader_() {
        }

        void Shader::load(std::string frag, std::string vert) {
            std::string vert_path = "shaders/vert/" + vert + ".vert";
            std::string frag_path = "shaders/frag/" + frag + ".frag";
            bool loaded = shader_.load(vert_path, frag_path);

            if (!loaded) {
                throw std::runtime_error("Unable to load shader with paths " + vert_path + " and " + frag_path);
            }
        }

        void Shader::setUniforms() {
            for (const auto& kv : params.getFloats()) {
                shader_.setUniform1f(kv.first, kv.second);
            }

            for (const auto& kv : params.getInts()) {
                shader_.setUniform1i(kv.first, kv.second);
            }

            for (const auto& kv : params.getBools()) {
                shader_.setUniform1i(kv.first, kv.second ? 1 : 0);
            }

            int tex_count = 1;
            for (const auto& kv : params.getTextures()) {
                shader_.setUniformTexture(kv.first, kv.second, tex_count++);
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
