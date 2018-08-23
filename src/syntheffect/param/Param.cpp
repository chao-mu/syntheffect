#include "syntheffect/param/Param.h"

namespace syntheffect {
    namespace param {
        Param Param::withValue(float v) const {
            Param p = *this;

            p.value = v;
            p.variable_value = "";

            return p;
        }

        Param Param::asDefault() const {
            Param p;

            p.name = name;
            p.cast = cast;
            p.value = default_value;

            return p;
        }

        Param Param::intValue(std::string name, int v) {
            Param p;

            p.name = name;
            p.value = v;
            p.cast = RoundIntCast;

            return p;
        }

        Param Param::floatValue(std::string name, float v) {
            Param p;

            p.name = name;
            p.value = v;

            return p;
        }

        Param Param::floatValue(std::string name, float v, float low, float high) {
            Param p;

            p.name = name;
            p.value = v;

            p.limits.exists = true;
            p.limits.value.low = low;
            p.limits.value.high = high;

            return p;
        }

        Param Param::boolValue(std::string name, bool v) {
            Param p;

            p.name = name;
            p.value = v ? 1 : -1;
            p.cast = PositiveIsTrueCast;

            return p;
        }

        bool Param::isVariable() const {
            return variable_value != "";
        }
    }
}
