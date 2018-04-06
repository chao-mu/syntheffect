#include "syntheffect/patch/PatchBuilder.h"

#include "syntheffect/effect/Shader.h"

#define DEFAULT_FRAG "Passthrough"
#define DEFAULT_VERT "Passthrough"
#define PIPELINE_PATH "//patch/pipeline"
#define CONTROLS_PATH "//patch/controls"

namespace syntheffect {
    namespace patch {
        shared_ptr<Patch> PatchBuilder::build(std::string path) {
            shared_ptr<Patch> p = make_shared<Patch>();

            ofXml xml = ofXml(path);

            // Set to pipeline
                
            if (xml.setTo(PIPELINE_PATH)) {
                if (!addPipeline(xml, p)) {
                    return nullptr;
                }

                xml.setToParent();
            } else {
                return nullptr;
            }

            // Set to controls
            if (xml.setTo(CONTROLS_PATH)) {
                if (!addControls(xml, p)) {
                    return nullptr;
                }

                xml.setToParent();
            } else {
                return nullptr;
            }

            return p;
        }

        bool PatchBuilder::addShader(ofXml& xml, shared_ptr<Patch> p) {
            shared_ptr<effect::Shader> shader = make_shared<effect::Shader>();

            std::string frag = xml.getAttribute("frag");
            if (frag.empty()) {
                frag = DEFAULT_FRAG;
            }

            std::string vert = xml.getAttribute("vert");
            if (vert.empty()) {
                vert = DEFAULT_VERT;
            }

            std::string id = xml.getAttribute("id");
            if (id.empty()) {
                ofLogError() << "XML shader definition missing id";

                return false;
            }

            ofLogNotice() << "Loading frag=" + frag + " vert=" + vert + "...";

            if (!shader->load(frag, vert)) {
                return false;
            }

            if (xml.getNumChildren() > 0) {
                xml.setToChild(0);
                do {
                    if (!addShaderParam(xml, shader)) {
                        return false;
                    }
                } while (xml.setToSibling());

                xml.setToParent();
            }

            p->addEffect(id, shader);

            return true;
        }

        bool PatchBuilder::addShaderParam(ofXml& xml, shared_ptr<effect::Shader> shader) {
            std::string param_el = xml.getName();
            std::string param_name = xml.getAttribute("name");

            if (param_el == "paramInt") {
                int v = xml.getIntValue("[@value]");
                auto func = [v](std::string name, ofShader& shad) { 
                    shad.setUniform1i(name, v);
                };
                shader->set(param_name, func);
            } else if (param_el == "paramFloat") {
                float v = xml.getFloatValue("[@value]");
                auto func = [v](std::string name, ofShader& shad) { 
                    shad.setUniform1f(name, v);
                };
                shader->set(param_name, func);
            } else {
                ofLogWarning() << "Unrecognized element " + param_el;
                return false;
            }

            return true;
        }

        bool PatchBuilder::addPipeline(ofXml& xml, shared_ptr<Patch> p) {
            // Iterate over pipeline
            if (xml.getNumChildren() > 0) {
                xml.setToChild(0);
                do {
                    std::string el_name = xml.getName();

                    if (el_name == "shader") {
                        if (!addShader(xml, p)) {
                            return false;
                        }
                    }
                } while (xml.setToSibling());
            }
            
            return true;
        }

        bool PatchBuilder::addControls(ofXml& xml, shared_ptr<Patch> p) {
            return true;
        }
    }
}

#undef DEFAULT_FRAG
#undef DEFAULT_VERT