#include "syntheffect/filter/ShaderFilter.h"

#include "ofGraphics.h"

#include "ofxYAML.h"

namespace syntheffect {
    namespace filter {
        ShaderFilter::ShaderFilter() : FilterBase(), shader_() {
        }

        bool ShaderFilter::load(std::string path) {
            ofxYAML conf;
            conf.load(path);
            std::string frag = conf["frag"].as<std::string>();
            std::string vert = conf["vert"].as<std::string>();
            shader_.load("shadersGL3/" + vert, "shadersGL3/" + frag);

            return true;
        }

        void ShaderFilter::draw(graphics::PingPongBuffer& ping_pong, float t) {
            if (!isActive()) {
                return;
            }

            ping_pong.dest->begin();
            ofClear(0, 255);

            ofTexture tex = ping_pong.src->getTexture();
            shader_.begin();
            
            shader_.setUniform2f("resolution", tex.getWidth(), tex.getHeight());
            shader_.setUniform1f("time", t);

            tex.draw(0, 0);

            shader_.end();
            ping_pong.dest->end();
        }
    }
}