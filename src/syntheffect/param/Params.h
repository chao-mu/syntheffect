#pragma once

#include <map>

#include "ofTexture.h"

namespace syntheffect {
    namespace param {
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

                void setParamButton(std::string which, std::string name);
                void setParamButtonTime(std::string which, std::string name, float offset);
                void setParamAxis(std::string name, std::string which, bool absolute, float low, float high);
                void setParamAxisPositive(std::string which, std::string name);
                void setParamAxisNegative(std::string which, std::string name);
                void setParamWaveCos(std::string name, float shift, float freq, float low, float high);
                void setParamWavePerlin(std::string name, float shift, float freq, float low, float high);
                void setParamWaveSin(std::string name, float shift, float freq, float low, float hight);

                void set(std::shared_ptr<Params> p);
        };
    }
}
