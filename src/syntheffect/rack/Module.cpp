#include "syntheffect/rack/Module.h"

namespace syntheffect {
    namespace rack {
        Module::Module(const std::string& id) : id_(id) {
        }

        void Module::start() {
        }

        bool Module::isReady() {
            return true;
        }

        void Module::setup(int /* width */, int /* height */, int /* internal_format */, const std::string& /* modules_dir */) {
        }

        std::shared_ptr<Channel> Module::getOutput(const std::string& name) {
            if (!output_channels_.count(name)) {
                throw std::runtime_error("Module " + id_ + " does not have output " + name);
            }

            return output_channels_.at(name);
        }

        void Module::setInput(const std::string& name, std::shared_ptr<Channel> input) {
            input_channels_[name] = input;
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
