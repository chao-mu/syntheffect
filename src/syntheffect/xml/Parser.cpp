#include "syntheffect/xml/Parser.h"

#include <cmath>
#include <string>

#include "ofMath.h"
#include "ofFileUtils.h"

#include "syntheffect/xml/Util.h"
#include "syntheffect/param/Parser.h"
#include "syntheffect/settings/Option.h"

#define DEFAULT_FRAG "Passthrough"
#define DEFAULT_VERT "Passthrough"

namespace syntheffect {
    namespace xml {
         settings::ProjectSettings Parser::parseProject(std::string path) {
            std::string root = ofFilePath::getEnclosingDirectory(path);
            ofXml xml;

            if (!xml.load(path)) {
               throw std::runtime_error("Unable to load project settings file " + path);
            }

            ofXml::Search search = xml.find("//project");
            if (search.empty()) {
                throw std::runtime_error(path + " is missing <project> section");
            }

            settings::ProjectSettings proj;

            xml = search.getFirst();

            proj.assets_path = ofFilePath::join(root, Util::getAttribute<std::string>(xml, "assets", false, "assets.xml"));
            proj.pipelines_path = ofFilePath::join(root, Util::getAttribute<std::string>(xml, "pipelines", false, "pipelines.xml"));
            proj.inputs_path = ofFilePath::join(root, Util::getAttribute<std::string>(xml, "inputs", false, "inputs.xml"));

            proj.pipelines = parsePipelines(proj.pipelines_path);

            proj.width = Util::getAttribute<int>(xml, "width", false, 1280);
            proj.height = Util::getAttribute<int>(xml, "height", false, 720);

            return proj;
         }

         std::vector<settings::PipelineSettings> Parser::parsePipelines(std::string path) {
            ofXml xml;

            if (!xml.load(path)) {
               throw std::runtime_error("Unable to load pipelines file " + path);
            }

            ofXml::Search pipelines_search = xml.find("//pipelines");
            if (pipelines_search.empty()) {
                throw std::runtime_error(path + " is missing <pipelines> section");
            }

            std::vector<settings::PipelineSettings> pipelines;
            // Iterate over pipeline
            for (const auto& child : pipelines_search.getFirst().getChildren()) {
               std::string el_name = child.getName();
                if (el_name == "pipeline") {
                  addPipeline(child, pipelines);
                } else {
                    throw std::runtime_error("Expecting pipeline, got <" + el_name + ">");
                }
            }

            return pipelines;
         }

        void Parser::addPipeline(const ofXml& xml, std::vector<settings::PipelineSettings>& pipelines) {
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

            pipelines.push_back(pipeline);
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

            shader.params.push_back(param::Parser::parseParam(xml));
        }
    }
}

#undef DEFAULT_FRAG
#undef DEFAULT_VERT
