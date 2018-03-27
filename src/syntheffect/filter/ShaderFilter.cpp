
#include "syntheffect/filter/ShaderFilter.h"

#include "ofGraphics.h"

namespace syntheffect {
    namespace filter {
        ShaderFilter::ShaderFilter(std::string name, std::string vert_name) : FilterBase() {
            shader_ = ofShader();
            shader_.load("shadersGL3/" + vert_name + ".vert", "shadersGL3/" + name + ".frag");;
        }

        void ShaderFilter::setupUniforms() {
        }

        void ShaderFilter::draw(graphics::PingPongBuffer& ping_pong, float t) {
            if (!isActive()) {
                return;
            }

            ping_pong.dest->begin();
            ofClear(0, 255);

            ofTexture tex = ping_pong.src->getTexture();
            shader_.begin();
            
            shader_.setUniformTexture("tex0", tex, 0);
            shader_.setUniform2f("resolution", tex.getWidth(), tex.getHeight());
            shader_.setUniform1f("time", t);

            setupUniforms();

            tex.draw(0, 0);
            shader_.end();
            ping_pong.dest->end();
        }
    }
}