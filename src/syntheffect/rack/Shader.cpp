#include "syntheffect/rack/Shader.h"

#include <math.h>

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

        void Shader::setup(int width, int height, int internal_format) {
            outputs_.allocate(width, height, internal_format);

            YAML::Node settings = YAML::LoadFile(ofFilePath::getAbsolutePath(path_));

            if (settings["inputs"]) {
                for (const auto& node : settings["inputs"]) {
                    input_names_.push_back(node.as<std::string>());
                }
            }

            YAML::Node outputs = settings["outputs"];
            float max_textures = std::ceil(((float) outputs.size()) / (float) CHANNELS_PER_TEX);
            for (std::size_t i=0; i < max_textures; i++) {
                outputs_.createAndAttachTexture(internal_format, i + 1);
            }

            for (std::size_t i=0; i < outputs.size(); i++) {
                int texture_idx = (i / CHANNELS_PER_TEX) + 1;
                output_channels_[outputs[i].as<std::string>()] = std::make_shared<Channel>(outputs_.getTexture(texture_idx), i % CHANNELS_PER_TEX);
            }

            accumulator_.allocate(width, height, internal_format);

            std::string frag = "Passthrough";
            if (settings["frag"]) {
                frag = settings["frag"].as<std::string>();
            }

            std::string vert = "Passthrough";
            if (settings["vert"]) {
                vert = settings["vert"].as<std::string>();
            }

            std::string vert_path = "shaders/vert/" + vert + ".vert";
            std::string frag_path = "shaders/frag/" + frag + ".frag";
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

            ofTexture& accum = accumulator_.dest().getTexture();
            outputs_.attachTexture(accum, accum.getTextureData().glInternalFormat, ACCUMULATOR_IDX);

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
            }

            shader_.setUniform1f("time", t);

            shader_.setUniform2f("resolution", outputs_.getWidth(), outputs_.getHeight());
            shader_.setUniformTexture("accumulatorIn", accumulator_.source(), idx++);
            shader_.setUniform1i("firstPass", first_pass_ ? 1 : 0);

            TS_START("Shader::update mesh_.draw");
            mesh_.draw();
            TS_STOP("Shader::update mesh_.draw");

            shader_.end();
            outputs_.end();

            accumulator_.swap();

            first_pass_ = false;
        }

        bool Shader::isReady() {
            return outputs_.isAllocated() && accumulator_.isAllocated();
        }
    }
}

#undef CHANNELS_PER_TEX
