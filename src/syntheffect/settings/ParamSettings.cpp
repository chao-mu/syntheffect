#include "syntheffect/settings/ParamSettings.h"

namespace syntheffect {
    namespace settings {
        ParamSettings ParamSettings::intValue(std::string name, int v) {
            ParamSettings p;

            p.name = name;
            p.value = v;
            p.continuous = true;

            return p;
        }

        ParamSettings ParamSettings::floatValue(std::string name, float v) {
            ParamSettings p;

            p.name = name;
            p.value = v;
            p.continuous = true;

            return p;
        }

        ParamSettings ParamSettings::floatValue(std::string name, float v, float low, float high) {
            ParamSettings p;

            p.name = name;
            p.low = low;
            p.high = high;
            p.value = v;

            return p;
        }

        ParamSettings ParamSettings::intValue(std::string name, int v, int low, int high) {
            ParamSettings p;

            p.name = name;
            p.low = low;
            p.high = high;
            p.value = v;
            p.cast = RoundIntCast;

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
