#pragma once

#include <string>

#include "syntheffect/settings/Option.h"

namespace syntheffect {
    namespace settings {
        struct ParamLimits {
            float low;
            float high;
        };

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

            Option<ParamLimits> limits;
            float value = 0;
            float shift = 0;
            float freq = 1;
            std::string variable_value;

            bool isVariable() const;

            static ParamSettings intValue(std::string name, int v);
            static ParamSettings floatValue(std::string name, float v, float low, float high);
            static ParamSettings floatValue(std::string name, float v);
            static ParamSettings boolValue(std::string name, bool v);
        };
    }
}
