#include "syntheffect/rack/Rack.h"

#include "boost/lexical_cast.hpp"

#include "ofGraphics.h"
#include "ofxTimeMeasurements.h"

#include "syntheffect/rack/Video.h"
#include "syntheffect/rack/Shader.h"
#include "syntheffect/rack/Global.h"
#include "syntheffect/rack/Joystick.h"
#include "syntheffect/rack/Carousel.h"
#include "syntheffect/rack/AudioAnalyzer.h"

#define VIDEO_MODULE "video"
#define JOYSTICK_MODULE "joystick"
#define AUDIO_ANALYZER_MODULE "audio_analyzer"
#define CAROUSEL_MODULE "carousel"
#define OUT_ID "out"
#define GLOBAL_ID "global"

namespace syntheffect {
    namespace rack {
        Rack::Rack(const std::string& path) : path_(path), is_ready_(false) {}

        void Rack::setup(int width, int height, size_t audio_buffer_size, int internal_format) {
            fbo_.allocate(width, height, internal_format);
            fbo_.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
            fbo_.begin();
            ofClear(0);
            fbo_.end();

            modules_[GLOBAL_ID] = std::make_shared<Global>(GLOBAL_ID);

            YAML::Node settings = YAML::LoadFile(path_);

            if (!settings[OUT_ID]) {
                throw std::runtime_error("No module found to produce final output, need module with id '" + std::string(OUT_ID) + "'");
            }

            const std::string rack_dir = ofFilePath::getEnclosingDirectory(path_);
            std::map<std::string, YAML::Node> carousels;

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
                        throw std::runtime_error("No path specified for module with id '" + id + "'. Use the property 'path'.");
                    }

                    std::string path = properties["path"].as<std::string>();
                    if (!ofFilePath::isAbsolute(path)) {
                        path = ofFilePath::join(rack_dir, path);
                    }

                    addModule(std::make_shared<Video>(id, path));
                } else if (type == AUDIO_ANALYZER_MODULE) {
                    if (!properties["path"]) {
                        throw std::runtime_error("No path specified for module with id '" + id + "'. Use the property 'path'.");
                    }

                    std::string path = properties["path"].as<std::string>();
                    if (!ofFilePath::isAbsolute(path)) {
                        path = ofFilePath::join(rack_dir, path);
                    }

                    auto audio = std::make_shared<AudioAnalyzer>(id, path, audio_buffer_size);
                    audio->connectTo(sound_output_);
                    addModule(audio);
                } else if (type == CAROUSEL_MODULE) {
                    carousels[id] = properties;
                } else if (type == JOYSTICK_MODULE) {
                    if (!properties["device"]) {
                        throw std::runtime_error(
                                "No device type specified for module with id '" + id + "'. Use the property 'device'.");
                    }

                    const std::string device = properties["device"].as<std::string>();

                    auto joy = std::make_shared<Joystick>(id, device);
                    joy_manager_.addJoystick(joy);
                    addModule(joy);
                } else {
                    const std::string path = "shaders/config/" + type + ".yml";
                    if (!ofFile::doesFileExist(path)) {
                        throw std::runtime_error("module type '" + type + "' is invalid.");
                    }


                    addModule(std::make_shared<Shader>(id, path));
                }
            }

            // Process now that all modules have been added. Currently doesn't support carousels with carousel children, boo
            for (const auto& id_and_properties : carousels) {
                auto id = id_and_properties.first;
                auto properties = id_and_properties.second;

                if (!properties["modules"]) {
                    throw std::runtime_error("No modules specified for module with id '" + id + "'. Use the property 'modules'.");
                }

                std::vector<std::shared_ptr<Module>> children;
                for (const auto node : properties["modules"]) {
                    std::string child_id = node.as<std::string>();

                    if (!modules_.count(child_id)) {
                        throw std::runtime_error(
                                "Module with id '" + id + "' specified non-existent module id '" + child_id + "'");
                    }

                    children.push_back(modules_.at(child_id));
                }

                addModule(std::make_shared<Carousel>(id, children));
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

                        to_module->setInput(to_channel_id, from_module->getOutput(from_channel_id));
                    }
                }
            }
        }

        ofBaseSoundOutput& Rack::getSoundOutput() {
            return sound_output_;
        }


        void Rack::addModule(std::shared_ptr<Module> module) {
            modules_[module->id_] = module;
        }

        bool Rack::updateUnready(float t) {
           bool all_ready = true;
            for (const auto& kv : modules_) {
                if (!kv.second->isReady()) {
                    kv.second->update(t);
                    all_ready = false;
                }
            }

            return all_ready;
        }

        void Rack::update(float t) {
            joy_manager_.update(t);

            TS_START("Rack::update update children");
            // Update global first
            auto global = modules_.at(GLOBAL_ID);
            global->update(t);
            for (const auto& kv : modules_) {
                if (kv.second != global) {
                    kv.second->update(t);
                }
            }
            TS_STOP("Rack::update update children");

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

        ofTexture& Rack::getTexture() {
            return fbo_.getTexture();
        }
    }
}

#undef COMPOSITE_MODULE
#undef CAROUSEL_MODULE
#undef VIDEO_MODULE
#undef OUT_NAME
#undef GLOBAL_NAME
