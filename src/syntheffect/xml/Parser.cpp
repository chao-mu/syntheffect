#include "syntheffect/xml/Parser.h"

#include <cmath>
#include <string>

#include "boost/lexical_cast.hpp"

#include "ofMath.h"

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
            settings::ProjectSettings project;
            // Iterate over pipeline
            for (const auto& child : xml.getChildren()) {
               std::string param_el = child.getName();
                if (param_el == "pipeline") {
                  addPipeline(child, project);
                } else {
                    throw std::runtime_error("Expecting pipeline, got <" + param_el + ">");
                }
            }

            m->setProject(project);
         }

        void Parser::addPipeline(const ofXml& xml, settings::ProjectSettings& project) {
            std::string in = xml.getAttribute("in").getValue();
            if (in.empty()) {
                throw std::runtime_error("Missing 'in' attribute in pipeline element");
            }

            std::string out = xml.getAttribute("out").getValue();
            if (out.empty()) {
                throw std::runtime_error("Missing 'out' attribute in pipeline element");
            }

            settings::PipelineSettings pipeline;
            pipeline.in = in;
            pipeline.out = out;

            // Iterate over pipeline
            for (const auto& child : xml.getChildren()) {
                std::string el_name = child.getName();

                if (el_name == "shader") {
                    addShader(child, pipeline);
                } else {
                    throw std::runtime_error("Expected <shader>, got <" + el_name + ">");
                }
            }

            project.pipelines.push_back(pipeline);
        }

        void Parser::addShader(const ofXml& xml, settings::PipelineSettings& pipeline) {
            std::string frag = xml.getAttribute("frag").getValue();
            if (frag.empty()) {
                frag = DEFAULT_FRAG;
            }

            std::string vert = xml.getAttribute("vert").getValue();
            if (vert.empty()) {
                vert = DEFAULT_VERT;
            }

            settings::ShaderSettings shader;
            shader.frag = frag;
            shader.vert = vert;

            for (const auto& child : xml.find("*")) {
                addShaderParam(child, shader);
            }

            pipeline.shaders.push_back(shader);
        }

        void Parser::addShaderParam(const ofXml& xml, settings::ShaderSettings& shader) {
            std::string param_el = xml.getName();

            if (param_el == "paramTexture") {
                std::string name = xml.getAttribute("name").getValue();
                std::string value = xml.getAttribute("value").getValue();

                shader.texture_params[name] = value;
                return;
            } else if (param_el != "param") {
                throw std::runtime_error("Expected <param>, got <" + param_el + ">");
            }

            std::string name = xml.getAttribute("name").getValue();
            if (name == "") {
                throw std::runtime_error("name attribute missing from <param>");
            }

            settings::ParamSettings p;
            p.name = name;

            std::string value = xml.getAttribute("value").getValue();
            try {
                p.value = boost::lexical_cast<float>(value);
            } catch (const boost::bad_lexical_cast &e) {
                p.variable_value = value;
            }

            std::string cast = xml.getAttribute("cast").getValue();
            if (cast == "round-int") {
                p.cast = settings::RoundIntCast;
            } else if (cast == "negative-is-true") {
                p.cast = settings::NegativeIsTrueCast;
            } else if (cast == "positive-is-true") {
                p.cast = settings::PositiveIsTrueCast;
            } else if (cast == "") {
                p.cast = settings::NoCast;
            } else {
                throw std::runtime_error("cast attribute missing or invalid in <param>");
            }

            std::string func = xml.getAttribute("func").getValue();
            if (func == "identity" || func == "") {
                p.func = settings::IdentityFunc;
            } else if (func == "noise") {
                p.func = settings::NoiseFunc;
            } else if (func == "cos") {
                p.func = settings::CosFunc;
            } else if (func == "sin") {
                p.func = settings::SinFunc;
            } else {
                throw std::runtime_error("type attribute missing or invalid in <param>");
            }

            std::string low = xml.getAttribute("low").getValue();
            if (low != "") {
                p.low = xml.getAttribute("low").getFloatValue();
            }

            std::string high = xml.getAttribute("high").getValue();
            if (high != "") {
                p.high = xml.getAttribute("high").getFloatValue();
            }

            std::string freq = xml.getAttribute("freq").getValue();
            if (freq != "") {
                p.freq = xml.getAttribute("freq").getFloatValue();
            }

            std::string shift = xml.getAttribute("shift").getValue();
            if (shift != "") {
                p.shift = xml.getAttribute("shift").getFloatValue();
            }

            shader.params.push_back(p);
        }
    }
}

#undef DEFAULT_FRAG
#undef DEFAULT_VERT
