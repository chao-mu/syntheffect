#include "syntheffect/xml/Parser.h"

#include <cmath>
#include <string>

#include "ofMath.h"

#include "syntheffect/graphics/Shader.h"

#define DEFAULT_FRAG "Passthrough"
#define DEFAULT_VERT "Passthrough"
#define PIPELINES_PATH "//patch/pipelines"
#define CONTROLS_PATH "//patch/controls"

namespace syntheffect {
    namespace xml {
        void Parser::parse(std::string path, std::shared_ptr<manager::Manager> m) {
            ofXml xml;

            if (!xml.load(path)) {
               throw std::runtime_error("Unable to load xml file " + path);
            }

            ofXml::Search pipelines_search = xml.find(PIPELINES_PATH);
            if (pipelines_search.empty()) {
                throw std::runtime_error(path + " is missing <pipelines> section");
            }

            if (pipelines_search.size() > 1) {
                throw std::runtime_error(path + "has more than one <pipelines> section");
            }

            ofXml pipeleines_section = pipelines_search.getFirst();
            addPipelines(pipeleines_section, m);
        }

        void Parser::addPipelines(const ofXml& xml, std::shared_ptr<manager::Manager> m) {
            // Iterate over pipeline
            for (const auto& child : xml.getChildren()) {
               std::string param_el = child.getName();
                if (param_el == "pipeline") {
                  addPipeline(child, m);
                } else {
                    throw std::runtime_error("Expecting pipeline, got <" + param_el + ">");
                }
            }
         }

        void Parser::addPipeline(const ofXml& xml, std::shared_ptr<manager::Manager> m) {
            std::string in = xml.getAttribute("in").getValue();
            if (in.empty()) {
                throw std::runtime_error("Missing 'in' attribute in pipeline element");
            }

            std::string out = xml.getAttribute("out").getValue();
            if (out.empty()) {
                throw std::runtime_error("Missing 'in' attribute in pipeline element");
            }

            int pipeline_id = m->addPipeline();
            m->connectPipelineOut(pipeline_id, out);
            m->connectPipelineIn(pipeline_id, in);

            // Iterate over pipeline
            for (const auto& child : xml.getChildren()) {
                std::string el_name = child.getName();

                if (el_name == "shader") {
                    addShader(child, m, pipeline_id);
                } else {
                    throw std::runtime_error("Expected <shader>, got <" + el_name + ">");
                }
            }
        }

        void Parser::addShader(const ofXml& xml, std::shared_ptr<manager::Manager> m, int pipeline_id) {
            std::string frag = xml.getAttribute("frag").getValue();
            if (frag.empty()) {
                frag = DEFAULT_FRAG;
            }

            std::string vert = xml.getAttribute("vert").getValue();
            if (vert.empty()) {
                vert = DEFAULT_VERT;
            }

            int effect_index = m->appendShaderEffect(pipeline_id, frag, vert);

            for (const auto& child : xml.find("*")) {
                addShaderParam(child, m, pipeline_id, effect_index);
            }
        }

        void Parser::addShaderParam(const ofXml& xml, std::shared_ptr<manager::Manager> m, int pipeline_id, int effect_index) {
            param::Params params;

            std::string param_el = xml.getName();
            std::string param_name = xml.getAttribute("name").getValue();

            if (param_el == "paramButton" || param_el == "paramPressed") {
                std::string which = xml.getAttribute("which").getValue();

                params.setParamPressed(param_name, which);
            } else if (param_el == "paramButtonTime" || param_el == "paramAxisTime" || param_el == "paramPressedTime") {
                std::string which = xml.getAttribute("which").getValue();

                float offset = 0;
                if (xml.getAttribute("offset").getValue() != "") {
                    offset = xml.getAttribute("offset").getFloatValue();
                }

                float speed = 1;
                if (xml.getAttribute("speed").getValue() != "") {
                    speed = xml.getAttribute("speed").getFloatValue();
                }

                params.setParamPressedTime(param_name, which, offset, speed);
            } else if (param_el ==  "paramAxisNegative") {
                std::string which = xml.getAttribute("which").getValue();

                params.setParamAxisNegative(param_name, which);
            } else if (param_el ==  "paramAxisPositive") {
                std::string which = xml.getAttribute("which").getValue();

                params.setParamAxisPositive(param_name, which);
            }  else if (param_el == "paramAxis") {
                std::string which = xml.getAttribute("which").getValue();
                bool absolute = false;
                if (xml.getAttribute("abs").getValue() == "") {
                    absolute = xml.getAttribute("abs").getFloatValue();
                }

                float low = -1;
                if (xml.getAttribute("low").getValue() != "") {
                    low = xml.getAttribute("low").getFloatValue();
                }

                float high = 1;
                if (xml.getAttribute("high").getValue() != "") {
                    high = xml.getAttribute("high").getFloatValue();
                }

                params.setParamAxis(param_name, which, absolute, low, high);
            }  else if (param_el == "paramInt") {
                int v = xml.getAttribute("value").getIntValue();
                params.set(param_name, v);
            } else if (param_el == "paramTime") {
                float speed = 1;
                if (xml.getAttribute("speed").getValue() != "") {
                    speed = xml.getAttribute("speed").getFloatValue();
                }

                std::function<float(param::ParamAccessors&)> f = [speed](param::ParamAccessors&) {
                    return ofGetElapsedTimef() * speed;
                };

                params.set(param_name, f);
            } else if (param_el == "paramFloat") {
                float v = xml.getAttribute("value").getFloatValue();
                params.set(param_name, v);
            } else if (param_el == "paramTexture") {
                std::string v = xml.getAttribute("value").getValue();
                params.set(param_name,  [v](param::ParamAccessors& p) {
                    return p.getTexture(v);
                });
            } else if (param_el == "paramBool") {
                std::string raw = xml.getAttribute("value").getValue();
                bool v = raw == "true" || raw == "on" || raw == "1";
                params.set(param_name, v);
            } else if (param_el == "paramWave") {
                float shift = 0;
                if (xml.getAttribute("shift").getValue() != "") {
                    shift = xml.getAttribute("shift").getFloatValue();
                }

                float freq = 1;
                if (xml.getAttribute("freq").getValue() != "") {
                    freq = xml.getAttribute("freq").getFloatValue();
                }

                float low = 0;
                if (xml.getAttribute("low").getValue() != "") {
                    low = xml.getAttribute("low").getFloatValue();
                }

                float high = 1;
                if (xml.getAttribute("high").getValue() != "") {
                    high = xml.getAttribute("high").getFloatValue();
                }

                std::string shape = xml.getAttribute("shape").getValue();
                if (shape == "cos") {
                    params.setParamWaveCos(param_name, shift, freq, low, high);
                } else if (shape == "sin") {
                    params.setParamWaveSin(param_name, shift, freq, low, high);
                } else if (shape == "perlin") {
                    params.setParamWavePerlin(param_name, shift, freq, low, high);
                } else {
                    throw std::runtime_error("Unspecified or invalid shape attribute: " + shape);
                }
            } else {
                throw std::runtime_error("Unrecognized element " + param_el);
            }

            m->setEffectParams(pipeline_id, effect_index, params);
        }
    }
}

#undef DEFAULT_FRAG
#undef DEFAULT_VERT
