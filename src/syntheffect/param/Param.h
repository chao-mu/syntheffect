#pragma once

#include <string>

#include "syntheffect/settings/Option.h"

namespace syntheffect {
    namespace param {
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

        struct Param {
            ParamCast cast = NoCast;
            ParamFunc func = IdentityFunc;

            std::string name;

            settings::Option<ParamLimits> limits;
            float value = 0;
            float default_value = 0;
            float shift = 0;
            float freq = 1;
            std::string variable_value;

            bool isVariable() const;

            static Param intValue(std::string name, int v);
            static Param floatValue(std::string name, float v, float low, float high);
            static Param floatValue(std::string name, float v);
            static Param boolValue(std::string name, bool v);

            Param withValue(float v) const;
            Param asDefault() const;
        };
    }
}
