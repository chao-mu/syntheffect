#include "syntheffect/midi/Device.h"

#include "ofMath.h"

#include "yaml-cpp/yaml.h"

#include "syntheffect/midi/Message.h"

namespace syntheffect {
    namespace midi {
        Device::Device(const std::string& id, const std::string& path) : Module(id), path_(path), midi_in_(new RtMidiIn()), running_(false) {
        }

        void Device::setup(int /* width */, int /* height */, int /* internal_format */, const std::string& /* modules_dir */) {
            load(path_);
            unsigned int port_count = midi_in_->getPortCount();
            bool found = false;
            for (unsigned int i = 0; i < port_count; i++) {
                std::string name = midi_in_->getPortName(i);
                if (std::regex_search(name, name_re_)) {
                    midi_in_->openPort(i);
                    found = true;
                    break;
                }
            }

            if (!found) {
                throw std::runtime_error("Requested midi device not found. Config loaded from " + path_);
            }
        }

        void Device::update(float /* t */) {
            std::lock_guard<std::mutex> guard(controls_mutex_);
            for (auto& kv : controls_) {
                Control& control = kv.second;
                if (control.type == CONTROL_TYPE_BUTTON) {
                    output_channels_[kv.first]->value_ = control.pressed ? 1 : 0;
                } else {
                    output_channels_[kv.first]->value_ = ofMap(control.value, control.low, control.high, 0, 1);
                }
            }
        }

        const std::string Device::getType() {
            return getModuleType();
        }

        const std::string Device::getModuleType() {
            return "core/midi";
        }


        void Device::start() {
            if (!running_) {
                running_ = true;
                thread_ = std::thread(&Device::loop, this);
            }
        }

        void Device::load(const std::string& path) {
            YAML::Node settings = YAML::LoadFile(path);
            if (!settings["regex"]) {
                throw std::runtime_error("Expected field 'regex' to be found in " + path);
            }

            name_re_ = std::regex(settings["regex"].as<std::string>());

            if (!settings["mappings"]) {
                throw std::runtime_error("Expected field 'mappings' to be found in " + path);
            }

            for (const auto& mapping : settings["mappings"]) {
                std::string name = mapping.first.as<std::string>();
                YAML::Node props = mapping.second;

                Control control;
                control.channel = (unsigned char) props["channel"].as<int>();
                control.function = (unsigned char) props["function"].as<int>();
                control.low = (unsigned char) props["low"].as<int>();
                control.high = (unsigned char) props["high"].as<int>();
                control.type = props["type"].as<std::string>() == "button" ? CONTROL_TYPE_BUTTON : CONTROL_TYPE_FADER;

                controls_[name] = control;
                output_channels_[name] =  std::make_shared<rack::Channel>(0);
            }
        }

        void Device::stop() {
            running_ = false;
            thread_.join();
        }

        void Device::loop() {
            while (running_.load()) {
                std::vector<unsigned char> raw_message;
                while (midi_in_->getMessage(&raw_message) > 0) {
                    Message msg(raw_message);

                    ControlType type = CONTROL_TYPE_UNKNOWN;
                    if (msg.getType() == MESSAGE_TYPE_NOTE_ON || msg.getType() == MESSAGE_TYPE_NOTE_OFF) {
                        type = CONTROL_TYPE_BUTTON;
                    } else if (msg.getType() == MESSAGE_TYPE_CONTROL) {
                        type = CONTROL_TYPE_FADER;
                    } else {
                        continue;
                    }

                    std::lock_guard<std::mutex> guard(controls_mutex_);
                    for (auto& kv : controls_) {
                        auto& control = kv.second;
                        if (control.function == msg.getFunction() && control.type == type && msg.getChannel() == control.channel) {
                            control.value = msg.getValue();
                        }
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
    }
}
