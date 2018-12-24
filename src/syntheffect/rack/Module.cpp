#include "syntheffect/rack/Module.h"

namespace syntheffect {
    namespace rack {
        Module::Module(const std::string& id) : id_(id) {
            output_groups_["rgb"] = {"red", "green", "blue"};
            output_groups_["rgba"] = {"red", "green", "blue", "alpha"};
            output_groups_["hsv"] = {"hue", "saturation", "value"};
            output_groups_["ycbcr"] = {"y", "cb", "cr"};

            input_groups_["rgb"] = {"red", "green", "blue"};
            input_groups_["rgba"] = {"red", "green", "blue", "alpha"};
            input_groups_["hsv"] = {"hue", "saturation", "value"};
            input_groups_["ycbcr"] = {"y", "cb", "cr"};
        }

        void Module::start() {
        }

        bool Module::isReady() {
            return true;
        }

        bool Module::inputExists(const std::string& name) {
            return false;
        }

        void Module::setup(int /* width */, int /* height */, int /* internal_format */, const std::string& /* workspace_dir */) {
        }

        bool Module::outputExists(const std::string& name) {
            return output_channels_.count(name) > 0;
        }

        void Module::stop() {

        }

        std::shared_ptr<Channel> Module::getOutput(const std::string& name) {
            if (!output_channels_.count(name)) {
                throw std::runtime_error("Module " + id_ + " does not have output " + name);
            }

            return output_channels_.at(name);
        }

        std::vector<std::shared_ptr<Channel>> Module::getOutputGroup(const std::string& name) {
            if (!output_groups_.count(name)) {
                throw std::runtime_error("Module " + id_ + " does not have output group " + name);
            }

            std::vector<std::shared_ptr<Channel>> outputs;
            for (const auto& k : output_groups_.at(name)) {
                if (!outputExists(k)) {
                    throw std::runtime_error("Output group " + id_ + "." + name + " referenced non-existent output " +  k);
                }

                outputs.push_back(getOutput(k));
            }

            return outputs;
        }

        void Module::setInput(const std::string& input_name, std::shared_ptr<Channel> input) {
            if (!inputExists(input_name)) {
                throw std::runtime_error("Input " + id_ + "." + input_name + " does not exist, yet was attempted to be set");
            }

            input_channels_[input_name] = input;
        }

        void Module::setInput(const std::string& input_name, std::shared_ptr<Module> src, const std::string output_name) {
            if (src->outputGroupExists(output_name)) {
                if (!input_groups_.count(input_name)) {
                    throw std::runtime_error("Tried to connect output group " +  src->id_ + "." + output_name + " to non-group input " + id_ + "." + input_name);
                }

                std::vector<std::shared_ptr<Channel>> output_group = src->getOutputGroup(output_name);
                std::vector<std::string> input_group = input_groups_.at(input_name);
                if (input_group.size() != output_group.size()) {
                    throw std::runtime_error("mismatched group sizes between input " + id_ + "." + input_name + " and output " + src->id_ + "." + output_name);
                }

                for (size_t i=0; i < input_group.size(); i++) {
                    const std::string& in = input_group.at(i);
                    if (!inputExists(in)) {
                        throw std::runtime_error("input group " + id_ + "." + input_name + " requires missing input '" + in + "'");
                    }

                    setInput(in, output_group.at(i));
                }
            } else {
                setInput(input_name, src->getOutput(output_name));
            }
        }

        bool Module::outputGroupExists(const std::string& name) {
            return output_groups_.count(name) > 0;
        }

        float Module::getInputConstant(const std::string& name, float def) {
            if (!input_channels_.count(name)) {
                return def;
            }

            auto chan = input_channels_.at(name);
            if (chan->texture_ != nullptr) {
                throw std::runtime_error(id_ + "." + name + " must be constant");
            }

            return chan->value_;
        }
    }
}
