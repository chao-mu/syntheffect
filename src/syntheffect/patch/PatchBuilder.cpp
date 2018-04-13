#include "syntheffect/patch/PatchBuilder.h"

#include "syntheffect/effect/Shader.h"

#define DEFAULT_FRAG "Passthrough"
#define DEFAULT_VERT "Passthrough"
#define PIPELINES_PATH "//patch/pipelines"
#define CONTROLS_PATH "//patch/controls"

namespace syntheffect {
    namespace patch {
        PatchBuilder::PatchBuilder() : effects_by_id_() {
        }

        shared_ptr<Patch> PatchBuilder::build(std::string path, shared_ptr<graphics::PingPongBufferMap> channels) {
            shared_ptr<Patch> p = make_shared<Patch>();

            ofXml xml = ofXml(path);

            // Set to pipeline
            if (xml.setTo(PIPELINES_PATH)) {
                if (!addPipelines(xml, p, channels)) {
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

        bool PatchBuilder::addShader(ofXml& xml, shared_ptr<graphics::PingPongBufferMap> channels, shared_ptr<Pipeline> parent) {
            shared_ptr<effect::Shader> shader = make_shared<effect::Shader>();

            std::string frag = xml.getAttribute("frag");
            if (frag.empty()) {
                frag = DEFAULT_FRAG;
            }

            std::string vert = xml.getAttribute("vert");
            if (vert.empty()) {
                vert = DEFAULT_VERT;
            }

            if (!shader->load(frag, vert)) {
                return false;
            }

            if (xml.getNumChildren() > 0) {
                xml.setToChild(0);
                do {
                    if (!addShaderParam(xml, channels, shader)) {
                        return false;
                    }
                } while (xml.setToSibling());

                xml.setToParent();
            }

            parent->addEffect(shader);

            std::string id = xml.getAttribute("id");
            if (!id.empty()) {
                effects_by_id_[id] = shader;
            }

            return true;
        }

        bool PatchBuilder::addShaderParam(ofXml& xml, shared_ptr<graphics::PingPongBufferMap> channels, shared_ptr<effect::Shader> parent) {
            std::string param_el = xml.getName();
            std::string param_name = xml.getAttribute("name");

            if (param_el == "paramInt") {
                int v = xml.getIntValue("[@value]");
                std::function<int()> f = [v]() { return v; };
                parent->setParam(param_name, f);
            } else if (param_el == "paramFloat") {
                float v = xml.getFloatValue("[@value]");
                std::function<float()> f = [v]() { return v; };
                parent->setParam(param_name, f);
            } else if (param_el == "paramTexture") {
                string v = xml.getValue("[@value]");
                std::function<ofTexture()> f = [v, channels]() {
                    return channels->get(v)->drawable()->getTexture();
                };
                parent->setParam(param_name, f);
            } else if (param_el == "paramBool") {
                std::string raw = xml.getValue("[@value]");
                bool v = raw == "true" || raw == "on" || raw == "1";
                std::function<bool()> f = [v]() { return v; };
                parent->setParam(param_name, f);
            } else {
                ofLogError() << "Unrecognized element " + param_el;
                return false;
            }

            return true;
        }

        bool PatchBuilder::addPipelines(ofXml& xml, shared_ptr<Patch> parent, shared_ptr<graphics::PingPongBufferMap> channels) {
            int children = xml.getNumChildren();
            if (children == 0) {
                ofLogError() << "<pipelines> requires children";
                return false;
            }

            // Iterate over pipeline
            xml.setToChild(0);
            for (int i=0; i < children; i++) {
                std::string param_el = xml.getName();
                if (param_el == "pipeline") {
                    if (!addPipeline(xml, parent, channels)) {
                        return false;
                    }
                } else {
                    ofLogError() << "Expecting pipeline, got " + param_el;
                    return false;
                }

                xml.setToSibling();
            }
            xml.setToParent();

            return true;
         }

        bool PatchBuilder::addPipeline(ofXml& xml, shared_ptr<Patch> patch, shared_ptr<graphics::PingPongBufferMap> channels) {
            int children = xml.getNumChildren();
            if (children == 0) {
                ofLogError() << "<pipeline> requires children";
                return false;
            }

            std::string in = xml.getAttribute("in");
            if (in.empty()) {
                ofLogError() << "Missing 'in' attribute in pipeline element";
                return false;
            }

            std::string out = xml.getAttribute("out");
            if (out.empty()) {
                ofLogError() << "Missing 'out' attribute in pipeline element";
                return false;
            }

            channels->allocate(out);

            shared_ptr<Pipeline> pipeline = make_shared<Pipeline>(in, out);

            // Iterate over pipeline
            xml.setToChild(0);
            for (int i=0; i < children; i++) {
                std::string el_name = xml.getName();

                if (el_name == "shader") {
                    if (!addShader(xml, channels, pipeline)) {
                        return false;
                    }
                }

                xml.setToSibling();
            }
            xml.setToParent();

            patch->addPipeline(pipeline);
            
            return true;
        }

        bool PatchBuilder::addControls(ofXml& xml, shared_ptr<Patch> p) {
            return true;
        }
    }
}

#undef DEFAULT_FRAG
#undef DEFAULT_VERT