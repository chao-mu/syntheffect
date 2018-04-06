#include "syntheffect/patch/Patch.h"

#include <string>

#include "ofUtils.h"

#include "ofXml.h"

#include "syntheffect/filter/ShaderFilter.h"

namespace syntheffect {
    namespace patch {
        Patch::Patch() : filters_() {
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

                    if (el_name == "effect") {
                        std::string name = conf.getAttribute("name");
                        std::string alias = conf.getAttribute("alias");
                        if (alias.empty()) {
                            alias = name;
                        }

                        shared_ptr<filter::ShaderFilter> effect = make_shared<filter::ShaderFilter>();
                        effect->load("shadersGL3/config/" + name + ".yaml");

                        filters_.push_back(effect);
                        filters_by_alias_[alias] = effect;

                        ofLogNotice() << "Loaded " + name;
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
            for (auto filter: filters_) {
                if (filter->isActive()) {
                    ping_pong.swap();
                    filter->draw(ping_pong, t);
                }
            }
        }
    }
}