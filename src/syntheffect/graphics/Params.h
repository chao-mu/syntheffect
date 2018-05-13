#pragma once

#include <map>

#include "ofTexture.h"

namespace syntheffect {
    namespace graphics {
        class Params {
            public:
                Params();

                std::map<std::string, float> float_params;
                std::map<std::string, int> int_params;
                std::map<std::string, bool> bool_params;
                std::map<std::string, std::function<float()>> float_func_params;
                std::map<std::string, std::function<int()>> int_func_params;
                std::map<std::string, std::function<bool()>> bool_func_params;
                std::map<std::string, std::function<ofTexture()>> texture_func_params;

                void set(std::shared_ptr<Params> p);
        };
    }
}
