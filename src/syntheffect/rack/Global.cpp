#include "syntheffect/rack/Global.h"

namespace syntheffect {
    namespace rack {
        Global::Global(const std::string& id, int width, int height) : Module(id), width_(width), height_(height) {
        }

        const std::string Global::getType() {
            return getModuleType();
        }

        const std::string Global::getModuleType() {
            return "video";
        }

        void Global::setup(int /* width */, int /* height */, int /* internal_format */) {
            output_channels_["width"] = std::make_shared<Channel>(width_);
            output_channels_["height"] = std::make_shared<Channel>(height_);
            output_channels_["time"] = std::make_shared<Channel>(0);
        }

        void Global::update(float t) {
            output_channels_.at("time")->value_ = t;
        }
    }
}
