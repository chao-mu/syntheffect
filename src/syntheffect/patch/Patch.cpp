#include "syntheffect/patch/Patch.h"

#include <string>

#include "ofUtils.h"

#include "ofXml.h"

#include "syntheffect/effect/Shader.h"

namespace syntheffect {
    namespace patch {
        Patch::Patch() : effects_() {
        }

        bool Patch::load(std::string path) {
            ofXml conf = ofXml(path);

            // Set to pipeline
            if (!conf.setTo("//patch/pipeline")) {
                return false;
            }

            // Iterate over pipeline
            if (conf.getNumChildren() > 0) {
                conf.setToChild(0);
                do {
                    std::string el_name = conf.getName();

                    if (el_name == "shader") {
                        std::string name = conf.getAttribute("name");
                        std::string alias = conf.getAttribute("alias");
                        if (alias.empty()) {
                            alias = name;
                        }

                        ofLogNotice() << "Loading " + name + "...";

                        shared_ptr<effect::Shader> shader = make_shared<effect::Shader>();
                        shader->load("shadersGL3/config/" + name + ".yaml");

                        int children = conf.getNumChildren();
                        if (children > 0) {
                            conf.setToChild(0);
                            do {
                                std::string child = conf.getName();
                                std::string param_name = conf.getAttribute("name");
                                if (child == "paramInt") {
                                    int v = conf.getIntValue("[@value]");
                                    auto func = [v](std::string name, ofShader& shad) { 
                                        shad.setUniform1i(name, v);
                                    };
                                    shader->set(param_name, func);
                                } else if (child == "paramFloat") {
                                    float v = conf.getFloatValue("[@value]");
                                    auto func = [v](std::string name, ofShader& shad) { 
                                        shad.setUniform1f(name, v);
                                    };
                                    shader->set(param_name, func);
                                }
                            } while (conf.setToSibling());
                            conf.setToParent();
                        }

                        effects_.push_back(shader);
                        effects_by_alias_[alias] = shader;

                    }
                } while (conf.setToSibling());
            }

            // Set to controls
            if (!conf.setTo("//patch/controls")) {
                ofLogError() << "Missing <controls> element under <patch>";
                return false;
            }

            if (conf.getNumChildren() > 0) {
                conf.setToChild(0);
                do {
                    std::string el_name = conf.getName();
                    ofLogNotice() << el_name;
                } while (conf.setToSibling());
            }

            return true;
        }

        void Patch::draw(graphics::PingPongBuffer& ping_pong) {
            float t = ofGetElapsedTimef();
            for (auto effect: effects_) {
                if (effect->isActive()) {
                    ping_pong.swap();
                    effect->draw(ping_pong, t);
                }
            }
        }
    }
}