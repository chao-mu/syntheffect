#include "syntheffect/param/Parser.h"

#include <string>
#include <stdexcept>

#include "boost/lexical_cast.hpp"

#include "syntheffect/xml//Util.h"

namespace syntheffect {
    namespace param {
         settings::ParamSettings Parser::parseParam(const ofXml& xml, bool require_default) {
            std::string name = xml::Util::getAttribute<std::string>(xml, "name", true, "");

            settings::ParamSettings p;

            std::string value = xml.getAttribute("value").getValue();
            if (value == "true") {
                p = settings::ParamSettings::boolValue(name, true);
            } else if (value == "false") {
                p = settings::ParamSettings::boolValue(name, false);
            } else {
                p.name = name;

                try {
                    p.value = boost::lexical_cast<float>(value);
                } catch (const boost::bad_lexical_cast &e) {
                    p.variable_value = value;
                }

                std::string cast = xml.getAttribute("cast").getValue();
                if (cast == "round-int") {
                    p.cast = settings::RoundIntCast;
                } else if (cast == "negative-is-true") {
                    p.cast = settings::NegativeIsTrueCast;
                } else if (cast == "positive-is-true") {
                    p.cast = settings::PositiveIsTrueCast;
                } else if (cast == "") {
                    p.cast = settings::NoCast;
                } else {
                    throw std::runtime_error("cast attribute missing or invalid in <param>");
                }

                std::string func = xml.getAttribute("func").getValue();
                if (func == "identity" || func == "") {
                    p.func = settings::IdentityFunc;
                } else if (func == "noise") {
                    p.func = settings::NoiseFunc;
                } else if (func == "cos") {
                    p.func = settings::CosFunc;
                } else if (func == "sin") {
                    p.func = settings::SinFunc;
                } else {
                    throw std::runtime_error("type attribute missing or invalid in <param>");
                }

                std::string low = xml.getAttribute("low").getValue();
                std::string high = xml.getAttribute("high").getValue();
                if (low != "" && high != "") {
                    settings::ParamLimits limits;
                    limits.low = xml.getAttribute("low").getFloatValue();
                    limits.high = xml.getAttribute("high").getFloatValue();
                    p.limits = settings::Option<settings::ParamLimits>::make(limits);
                }

                std::string freq = xml.getAttribute("freq").getValue();
                if (freq != "") {
                    p.freq = xml.getAttribute("freq").getFloatValue();
                }

                std::string shift = xml.getAttribute("shift").getValue();
                if (shift != "") {
                    p.shift = xml.getAttribute("shift").getFloatValue();
                }
            }

            std::string default_value = xml::Util::getAttribute<std::string>(xml, "default", require_default, "");
            if (default_value == "true") {
                p.default_value = p.cast == settings::PositiveIsTrueCast ? 1 : -1;
            } else if (default_value == "false") {
                p.default_value = p.cast == settings::PositiveIsTrueCast ? -1 : 1;
            } else if (default_value != "") {
                try {
                    p.default_value = boost::lexical_cast<float>(default_value);
                } catch (const boost::bad_lexical_cast &e) {
                    throw std::runtime_error("param " + name + " has incorrect type of value for attribute default ");
                }
            }

            return p;
         }
    }
}
