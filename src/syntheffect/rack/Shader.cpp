#include "syntheffect/rack/Shader.h"

#include <math.h>
#include <regex>
#include <string>

#include "yaml-cpp/yaml.h"

#include "ofGraphics.h"
#include "ofPoint.h"

#include "ofxTimeMeasurements.h"

#include "syntheffect/rack/Channel.h"

#define CHANNELS_PER_TEX 3
#define ACCUMULATOR_IDX 0

namespace syntheffect {
    namespace rack {
        Shader::Shader(const std::string& id, const std::string& path) : Module(id), path_(path), first_pass_(true) {
        }

        const std::string Shader::getType() {
            return getModuleType();
        }

        const std::string Shader::getModuleType() {
            return "builtin/shader";
        }

        bool Shader::inputExists(const std::string& name) {
            return input_names_.find(name) != input_names_.end();
        }

        void Shader::setup(int width, int height, int internal_format, const std::string& workspace_dir) {
            ofBuffer buffer = ofBufferFromFile(path_);

            std::vector<std::string> outputs;
            std::regex input_re(R"(DEFINE_INPUT\s*\(\s*(\w+))");
            std::regex output_re(R"(DEFINE_OUTPUT_\d+\s*\(\s*(\w+))");
            //std::regex input_rgb_with_re(R"(DEFINE_INPUTS_(RGBA?)_WITH\(\s*(\w+))");
            std::regex input_rgb_as_re(R"(DEFINE_INPUTS_(RGBA?)_AS\(\s*(\w+))");
            std::regex input_rgb_re(R"(DEFINE_INPUTS_(RGBA?)\()");
            std::regex output_rgb_re(R"(DEFINE_OUTPUTS_(RGBA?)_1234?\()");
            for (auto line : buffer.getLines()){
                std::smatch matches;
                if (std::regex_search(line, matches, input_re)) {
                    input_names_.insert(matches[1]);
                } else if (std::regex_search(line, matches, output_re)) {
                    outputs.push_back(matches[1]);
                } else if (std::regex_search(line, matches, output_rgb_re)) {
                    outputs.push_back("red");
                    outputs.push_back("green");
                    outputs.push_back("blue");
                    if (matches[1] == "RGBA") {
                        outputs.push_back("alpha");
                    }
                } else if (std::regex_search(line, matches, input_rgb_re)) {
                    input_names_.insert("red");
                    input_names_.insert("green");
                    input_names_.insert("blue");
                    if (matches[1] == "RGBA") {
                        input_names_.insert("alpha");
                    }
                } else if (std::regex_search(line, matches, input_rgb_as_re)) {
                    std::string name = matches[2];
                    input_names_.insert(name + "_red");
                    input_names_.insert(name + "_green");
                    input_names_.insert(name + "_blue");
                    input_groups_[name] = {name + "_red", name + "_green", name + "_blue"};
                    if (matches[1] == "RGBA") {
                        input_names_.insert(name + "_alpha");
                        input_groups_.at(name).push_back(name + "_alpha");
                    }
                }
            }

            input_names_.insert("syncH");
            input_names_.insert("syncV");

            int texture_count = std::ceil(((float) outputs.size()) / (float) CHANNELS_PER_TEX);
            outputs_.allocate(width, height, internal_format);
            last_outputs_.resize(texture_count);
            for (int i=0; i < texture_count; i++) {
                ofFbo& last_output = last_outputs_.at(i);
                last_output.allocate(width, height, internal_format);
                last_output.begin();
                ofClear(0);
                last_output.end();
                outputs_.createAndAttachTexture(internal_format, i);
            }

            for (std::size_t i=0; i < outputs.size(); i++) {
                int texture_idx = i / CHANNELS_PER_TEX;
                output_channels_[outputs[i]] =
                    std::make_shared<Channel>(outputs_.getTexture(texture_idx), i % CHANNELS_PER_TEX);
            }

            std::string frag_path = path_;
            std::string vert_path = ofFilePath::join(workspace_dir, ofFilePath::join("shaders", "Passthrough.vert"));
            if (!shader_.load(vert_path, frag_path)) {
                throw std::runtime_error("Unable to load shader with paths " + vert_path + " and " + frag_path);
            }

            mesh_.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

            mesh_.addVertex(ofPoint(0,0));
            mesh_.addTexCoord({0, 0});

            mesh_.addVertex(ofPoint(width, 0));
            mesh_.addTexCoord({width, 0});

            mesh_.addVertex(ofPoint(0, height));
            mesh_.addTexCoord({0, height});

            mesh_.addVertex(ofPoint(width, height));
            mesh_.addTexCoord({width, height});
        }

        void Shader::update(float t) {
            std::map<ofTexture*, int> textures;

            TSGL_START("Shader render");

            outputs_.begin();
            outputs_.activateAllDrawBuffers();

            shader_.begin();

            int idx = 0;
            for (const std::string& name : input_names_) {
                if (input_channels_.count(name)) {
                    std::shared_ptr<Channel> channel = input_channels_.at(name);
                    if (channel->texture_ == nullptr) {
                        shader_.setUniform1i(name + "PropertyPassed", 1);
                        shader_.setUniform1f(name + "PropertyValue", channel->value_);
                    } else {
                        shader_.setUniform1i(name + "PropertyPassed", 0);
                        ofTexture* tex = channel->texture_;
                        if (!textures.count(channel->texture_)) {
                            shader_.setUniformTexture("inputs" + std::to_string(idx), *tex, idx);
                            textures[tex] = idx;
                            idx++;
                        }

                        shader_.setUniform1i(name + "TexIdx", textures.at(tex));
                        shader_.setUniform1i(name + "ChannelIdx", channel->idx_);
                    }
                } else {
                    shader_.setUniform1i(name + "TexIdx", -1);
                    shader_.setUniform1i(name + "ChannelIdx", -1);
                    shader_.setUniform1i(name + "PropertyPassed", 0);
                }

                std::string multiplier_name = name + "_multiplier";
                shader_.setUniform1f(multiplier_name, getInputConstant(multiplier_name, 1));

                std::string shift_name = name + "_shift";
                shader_.setUniform1f(shift_name, getInputConstant(shift_name, 0));

                std::string invert_name = name + "_invert";
                shader_.setUniform1f(invert_name, getInputConstant(invert_name, 0));
            }

            for (size_t i=0; i < last_outputs_.size(); i++) {
                shader_.setUniformTexture("lastOutput" + std::to_string(i), last_outputs_.at(i), idx++);
            }

            shader_.setUniform1f("time", t);

            shader_.setUniform2f("resolution", outputs_.getWidth(), outputs_.getHeight());
            shader_.setUniform1i("firstPass", first_pass_ ? 1 : 0);

            mesh_.draw();

            shader_.end();
            outputs_.end();
            TSGL_STOP("Shader render");

            TSGL_START("Copying outputs");
            for (int i=0; i < outputs_.getNumTextures(); i++) {
                ofFbo& buf = last_outputs_.at(i);
                buf.begin();
                outputs_.getTexture(i).draw(0, 0);
                //ofClear(1);
                buf.end();
            }
            TSGL_STOP("Copying outputs");

            first_pass_ = false;
        }

        bool Shader::isReady() {
            return outputs_.isAllocated();
        }
    }
}

#undef CHANNELS_PER_TEX
