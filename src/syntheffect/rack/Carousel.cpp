#include "syntheffect/rack/Carousel.h"

#include <algorithm>

#include "syntheffect/rack/Channel.h"

#define THRESHOLD_DEFAULT 0.5
#define NEXT_PREFIX "next_"
#define PREV_PREFIX "prev_"

namespace syntheffect {
    namespace rack {
        Carousel::Carousel(const std::string& id, std::vector<std::shared_ptr<Module>> modules) : Module(id), modules_(modules) {
        }

        std::shared_ptr<Channel> Carousel::getOutput(const std::string& name) {
            output_channels_[name] = std::make_shared<Channel>(0);

            return Module::getOutput(name);
        }

        const std::string Carousel::getType() {
            return getModuleType();
        }

        const std::string Carousel::getModuleType() {
            return "core/carousel";
        }

        void Carousel::start() {
            updateOutputs();
        }

        void Carousel::update(float /* t */) {
            float prev_val = getInputConstant("prev", 0);
            float prev_thresh = getInputConstant("prev_threshold", THRESHOLD_DEFAULT);
            if (prev_trigger_value_ < prev_thresh && prev_val > prev_thresh) {
                if (modules_.size() > 1) {
					std::rotate(modules_.rbegin(), modules_.rbegin() + 1, modules_.rend());
                    updateOutputs();
                }
            }

            float next_val = getInputConstant("next", 0);
            float next_thresh = getInputConstant("next_threshold", THRESHOLD_DEFAULT);
            if (next_trigger_value_ < next_thresh && next_val > next_thresh) {
                if (modules_.size() > 1) {
                    std::rotate(modules_.begin(), modules_.begin() + 1, modules_.end());
                    updateOutputs();
                }
            }

            prev_trigger_value_ = prev_val;
            next_trigger_value_ = next_val;
        }

        void Carousel::updateOutputs() {
            for (const auto& kv : output_channels_) {
                auto name = kv.first;
                std::string::size_type next_pos = name.find(NEXT_PREFIX);
                if (next_pos != std::string::npos) {
                    getNext()->getOutput(name.substr(next_pos + std::string(NEXT_PREFIX).length()))->copyTo(*kv.second.get());
                    continue;
                }

                std::string::size_type prev_pos = name.find(PREV_PREFIX);
                if (prev_pos != std::string::npos) {
                    getNext()->getOutput(name.substr(prev_pos + std::string(PREV_PREFIX).length()))->copyTo(*kv.second.get());
                    continue;
                }

                getCurrent()->getOutput(name)->copyTo(*kv.second.get());
            }
        }

        std::shared_ptr<Module> Carousel::getCurrent() {
            return *modules_.begin();
        }

        std::shared_ptr<Module> Carousel::getNext() {
            return modules_[1 % modules_.size()];
        }

        std::shared_ptr<Module> Carousel::getPrev() {
            return modules_[modules_.size() - 1];
        }
    }
}

#undef THRESHOLD_DEFAULT
#undef NEXT_PREFIX
#undef PREV_PREFIX
