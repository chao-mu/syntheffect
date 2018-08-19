#include "syntheffect/settings/ParamSettings.h"

namespace syntheffect {
    namespace settings {
        ParamSettings ParamSettings::withValue(float v) const {
            ParamSettings p = *this;

            p.value = v;
            p.variable_value = "";

            return p;
        }

        ParamSettings ParamSettings::asDefault() const {
            ParamSettings p;

            p.name = name;
            p.cast = cast;
            p.value = default_value;

            return p;
        }

        ParamSettings ParamSettings::intValue(std::string name, int v) {
            ParamSettings p;

            p.name = name;
            p.value = v;
            p.cast = RoundIntCast;

            return p;
        }

        ParamSettings ParamSettings::floatValue(std::string name, float v) {
            ParamSettings p;

            p.name = name;
            p.value = v;

            return p;
        }

        ParamSettings ParamSettings::floatValue(std::string name, float v, float low, float high) {
            ParamSettings p;

            p.name = name;
            p.value = v;

            p.limits.exists = true;
            p.limits.value.low = low;
            p.limits.value.high = high;

            return p;
        }

        ParamSettings ParamSettings::boolValue(std::string name, bool v) {
            ParamSettings p;

            p.name = name;
            p.value = v ? 1 : -1;
            p.cast = PositiveIsTrueCast;

            return p;
        }

        bool ParamSettings::isVariable() const {
            return variable_value != "";
        }
    }
}
