#pragma once

#include <string>

namespace syntheffect {
    namespace settings {
        enum ParamCast {
            RoundIntCast, NegativeIsTrueCast, PositiveIsTrueCast, NoCast
        };

        enum ParamFunc {
            IdentityFunc, NoiseFunc, CosFunc, SinFunc
        };

        struct ParamSettings {
            ParamCast cast = NoCast;
            ParamFunc func = IdentityFunc;

            std::string name;

            float low = -1;
            float high = 1;
            float value = 0;
            float shift = 0;
            float freq = 1;
            bool continuous = false;
            std::string variable_value;

            bool isVariable() const;

            static ParamSettings intValue(std::string name, int v, int low, int high);
            static ParamSettings intValue(std::string name, int v);
            static ParamSettings floatValue(std::string name, float v, float low, float high);
            static ParamSettings floatValue(std::string name, float v);
            static ParamSettings boolValue(std::string name, bool v);
        };

    }
}
