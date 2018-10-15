#include "syntheffect/rack/Rack.h"

#include "boost/lexical_cast.hpp"
#include "yaml-cpp/yaml.h"

#include "ofGraphics.h"

#include "syntheffect/rack/Video.h"
#include "syntheffect/rack/Shader.h"

#define VIDEO_MODULE "video"
#define OUT_ID "out"

namespace syntheffect {
    namespace rack {
        Rack::Rack(const std::string& path) : path_(path), is_ready_(false) {}

        void Rack::setup(int width, int height, int internal_format) {
            fbo_.allocate(width, height, internal_format);
            fbo_.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
            fbo_.begin();
            ofClear(0);
            fbo_.end();

            YAML::Node settings = YAML::LoadFile(path_);

            if (!settings[OUT_ID]) {
                throw std::runtime_error("No module found to produce final output, need module with id '" + std::string(OUT_ID) + "'");
            }

            const std::string rack_dir = ofFilePath::getEnclosingDirectory(path_);

            // Add all modules
            for (const auto& kv : settings) {
                std::string id = kv.first.as<std::string>();
                YAML::Node properties = kv.second;

                if (!properties["module"]) {
                    throw std::runtime_error("No module type specified for module with id '" + id + "'. Use the property 'module'.");
                }

                const std::string type = properties["module"].as<std::string>();
                if (type == VIDEO_MODULE) {
                    if (!properties["path"]) {
                        throw std::runtime_error("No path specified for video module with id '" + id + "'. Use the property 'module'.");
                    }

                    std::string path = properties["path"].as<std::string>();
                    if (!ofFilePath::isAbsolute(path)) {
                        path = ofFilePath::join(rack_dir, path);
                    }

                    addModule(std::make_shared<Video>(id, path));
                } else {
                    const std::string path = "shaders/config/" + type + ".yml";

                    addModule(std::make_shared<Shader>(id, path));
                }
            }

            for (const auto& kv : modules_) {
                kv.second->setup(width, height, internal_format);
            }

            // Add all connections now that modules are added.
            for (const auto& id_props : settings) {
                const std::string to_module_id = id_props.first.as<std::string>();
                if (!modules_.count(to_module_id)) {
                    throw std::runtime_error("Connection requested TO non existent module of id '" + to_module_id + "'");
                }
                auto to_module = modules_.at(to_module_id);

                YAML::Node properties = id_props.second;

                if (!properties["inputs"]) {
                    continue;
                }

                ofLogNotice("Rack", "processing inputs for " + to_module_id);

                for (const auto& kv : properties["inputs"]) {
                    const std::string to_channel_id = kv.first.as<std::string>();

                    const std::string value_str = kv.second.as<std::string>();
                    float value_f = 0;
                    bool is_float = true;
                    try {
                        value_f = boost::lexical_cast<float>(value_str);
                    } catch(boost::bad_lexical_cast &) {
                        is_float = false;
                    }

                    if (is_float) {
                        to_module->setInput(to_channel_id, std::make_shared<Channel>(value_f));
                    } else {
                        auto dot = value_str.find(".");
                        if (dot == std::string::npos) {
                            throw std::runtime_error("Invalid value for input " + to_module_id + "." + to_channel_id);
                        }

                        const std::string from_module_id = value_str.substr(0, dot);
                        const std::string from_channel_id = value_str.substr(dot + 1);
                        if (!modules_.count(from_module_id)) {
                            throw std::runtime_error("Failed to connect non-existent module '" + from_module_id + "' to " + to_module_id + "." + to_channel_id);
                        }

                        auto from_module = modules_.at(from_module_id);

                        ofLogNotice("Rack", "Setting  input "  + from_module_id + "." + from_channel_id);
                        to_module->setInput(to_channel_id, from_module->getOutput(from_channel_id));
                    }
                }
            }
        }

        void Rack::addModule(std::shared_ptr<Module> module) {
            modules_[module->id_] = module;
        }

        void Rack::update(float t) {
            // Update those not ready;
            bool all_ready = true;
            for (const auto& kv : modules_) {
                if (!kv.second->isReady()) {
                    kv.second->update(t);
                    all_ready = false;
                }
            }

            if (!all_ready) {
                return;
            }

            for (const auto& kv : modules_) {
                kv.second->update(t);
            }

            auto out = modules_.at(OUT_ID);
            auto r = out->getOutput("red");
            auto g = out->getOutput("green");
            auto b = out->getOutput("blue");

            if (r->texture_ != g->texture_ || g->texture_ != b->texture_) {
                // We need them to be stored in the same texture, but that's abstracted away so we lie about requirements
                throw std::runtime_error("Output module '" + std::string(OUT_ID) + "' must have first three output channels named red green blue. The fourth input determines alpha");
            }

            // Remeber this is the same texture of all
            fbo_.begin();
            r->texture_->draw(0, 0);
            fbo_.end();
        }

        void Rack::start() {
            for (const auto& kv : modules_) {
                kv.second->start();
            }
        }

        bool Rack::isReady() {
            if (is_ready_) {
                return true;
            }

            for (const auto& kv : modules_) {
                if (!kv.second->isReady()) {
                    return false;
                }
            }

            is_ready_ = true;

            return true;
        }

        ofTexture& Rack::getTexture() {
            return fbo_.getTexture();
        }
    }
}

#undef COMPOSITE_MODULE
#undef VIDEO_MODULE
#undef OUT_NAME
