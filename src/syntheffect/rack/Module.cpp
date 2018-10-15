#include "syntheffect/rack/Module.h"

#include "ofLog.h"

namespace syntheffect {
    namespace rack {
        Module::Module(const std::string& id) : id_(id) {
        }

        void Module::start() {
        }

        bool Module::isReady() {
            return true;
        }

        std::shared_ptr<Channel> Module::getOutput(const std::string& name) {
            if (!output_channels_.count(name)) {
                throw std::runtime_error("Shader " + id_ + " does not have output " + name);
            }

            //ofLogNotice("Module", "name=%s", name.c_str());

            return output_channels_.at(name);
        }

        void Module::setInput(const std::string& name, std::shared_ptr<Channel> input) {
            input_channels_[name] = input;
        }

    }
}
