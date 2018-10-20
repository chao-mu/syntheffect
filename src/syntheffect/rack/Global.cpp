#include "syntheffect/rack/Global.h"

namespace syntheffect {
    namespace rack {
        Global::Global(const std::string& id) : Module(id) {
        }

        void Global::setup(int width, int height, int /* internal_format */) {
            output_channels_["width"] = std::make_shared<Channel>(width);
            output_channels_["height"] = std::make_shared<Channel>(height);
            output_channels_["time"] = std::make_shared<Channel>(0);
        }

        void Global::update(float t) {
            output_channels_.at("time")->value_ = t;
        }
    }
}
