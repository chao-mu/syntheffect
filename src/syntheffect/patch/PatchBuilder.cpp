#include "syntheffect/patch/PatchBuilder.h"

#include <cmath>

#include "syntheffect/graphics/Shader.h"

#define DEFAULT_FRAG "Passthrough"
#define DEFAULT_VERT "Passthrough"
#define PIPELINES_PATH "//patch/pipelines"
#define CONTROLS_PATH "//patch/controls"

namespace syntheffect {
    namespace patch {
        PatchBuilder::PatchBuilder() : effects_by_id_() {
        }

        std::shared_ptr<Patch> PatchBuilder::build(std::string path, std::shared_ptr<graphics::PingPongBufferMap> channels) {
            std::shared_ptr<Patch> p = std::make_shared<Patch>();

            ofXml xml;

            if (!xml.load(path)) {
                ofLogError("PatchBuilder", "Unable to load xml file %s", path.c_str());
                return nullptr;
            }

            ofXml::Search pipelines_search = xml.find(PIPELINES_PATH);
            if (pipelines_search.empty()) {
                ofLogError("PatchBuilder", "Missing <pipelines> section");
                return nullptr;
            }

            if (pipelines_search.size() > 1) {
                ofLogError("PatchBulder", "More than one <pipelines> section found");
                return nullptr;
            }

            ofXml pipeleines_section = pipelines_search.getFirst();
            if (!addPipelines(pipeleines_section,  p, channels)) {
                return nullptr;
            }

            return p;
        }

        bool PatchBuilder::addShader(const ofXml& xml, std::shared_ptr<graphics::PingPongBufferMap> channels, std::shared_ptr<Pipeline> parent) {
            std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>();

            std::string frag = xml.getAttribute("frag").getValue();
            if (frag.empty()) {
                frag = DEFAULT_FRAG;
            }

            std::string vert = xml.getAttribute("vert").getValue();
            if (vert.empty()) {
                vert = DEFAULT_VERT;
            }

            if (!shader->load(frag, vert)) {
                return false;
            }

            for (const auto& child : xml.find("*")) {
                if (!addShaderParam(child, channels, shader)) {
                    return false;
                }
            }

            parent->addEffect(shader);

            return true;
        }

        bool PatchBuilder::addShaderParam(const ofXml& xml, std::shared_ptr<graphics::PingPongBufferMap> channels, std::shared_ptr<graphics::Shader> parent) {
            std::string param_el = xml.getName();
            std::string param_name = xml.getAttribute("name").getValue();

            if (param_el == "paramInt") {
                int v = xml.getAttribute("value").getIntValue();
                parent->params.int_params[param_name] = v;
            } else if (param_el == "paramTime") {
                float speed = 1;
                if (xml.getAttribute("speed").getValue() != "") {
                    speed = xml.getAttribute("speed").getFloatValue();
                }

                std::function<float()> f = [speed]() {
                    return ofGetElapsedTimef() * speed;
                };

                parent->params.float_func_params[param_name] = f;
            } else if (param_el == "paramFloat") {
                float v = xml.getAttribute("value").getFloatValue();
                parent->params.float_params[param_name] = v;
            } else if (param_el == "paramTexture") {
                std::string v = xml.getAttribute("value").getValue();
                std::function<ofTexture()> f = [v, channels]() {
                    return channels->get(v)->drawable()->getTexture();
                };
                parent->params.texture_func_params[param_name] = f;
            } else if (param_el == "paramBool") {
                std::string raw = xml.getAttribute("value").getValue();
                bool v = raw == "true" || raw == "on" || raw == "1";
                parent->params.bool_params[param_name] = v;
            } else if (param_el == "paramWave") {
                float shift = 0;
                if (xml.getAttribute("shift").getValue() != "") {
                    shift = xml.getAttribute("shift").getFloatValue();
                }

                float amplitude = 1;
                if (xml.getAttribute("amp").getValue() != "") {
                    amplitude = xml.getAttribute("amp").getFloatValue();
                }

                float freq = 1;
                if (xml.getAttribute("freq").getValue() != "") {
                    freq = xml.getAttribute("freq").getFloatValue();
                }

                float offset_y = 0;
                if (xml.getAttribute("offset-y").getValue() != "") {
                    offset_y = xml.getAttribute("offset-y").getFloatValue();
                }

                std::function<float(float)> wave;
                std::string shape = xml.getAttribute("shape").getValue();
                if (shape == "cos") {
                    wave = cosf;
                } else if (shape == "sin") {
                    wave = sinf;
                } else {
                    ofLogError("PatchBuilder", "Unspecified or invalid shape attribute: %s", shape.c_str());
                    return false;
                }

                std::function<float()> f = [wave, shift, amplitude, freq, offset_y]() {
                    return offset_y + ((1.0 + wave((ofGetElapsedTimef() * freq) + shift) ) * 0.5 * amplitude);
                };

                parent->params.float_func_params[param_name] = f;
            } else {
                ofLogError("PatchBuilder", "Unrecognized element %s", param_el.c_str());
                return false;
            }

            return true;
        }

        bool PatchBuilder::addPipelines(const ofXml& xml, std::shared_ptr<Patch> parent, std::shared_ptr<graphics::PingPongBufferMap> channels) {
            // Iterate over pipeline
            for (const auto& child : xml.getChildren()) {
               std::string param_el = child.getName();
                if (param_el == "pipeline") {
                    if (!addPipeline(child, parent, channels)) {
                        return false;
                    }
                } else {
                    ofLogError("PatchBuilder", "Expecting pipeline, got %s", param_el.c_str());
                    return false;
                }
            }

            return true;
         }

        bool PatchBuilder::addPipeline(const ofXml& xml, std::shared_ptr<Patch> patch, std::shared_ptr<graphics::PingPongBufferMap> channels) {
            std::string in = xml.getAttribute("in").getValue();
            if (in.empty()) {
                ofLogError("PatchBuilder", "Missing 'in' attribute in pipeline element");
                return false;
            }

            std::string out = xml.getAttribute("out").getValue();
            if (out.empty()) {
                ofLogError("PatchBuilder", "Missing 'out' attribute in pipeline element");
                return false;
            }

            channels->allocate(out);

            std::shared_ptr<Pipeline> pipeline = std::make_shared<Pipeline>(in, out);
            patch->addPipeline(pipeline);

            // Iterate over pipeline
            for (const auto& child : xml.getChildren()) {
                std::string el_name = child.getName();

                if (el_name == "shader") {
                    if (!addShader(child, channels, pipeline)) {
                        return false;
                    }
                }
            }

            return true;
        }
    }
}

#undef DEFAULT_FRAG
#undef DEFAULT_VERT
