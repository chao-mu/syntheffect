#include "boost/lexical_cast.hpp"

namespace syntheffect {
    namespace xml {
        template<typename T>
        T Util::getAttribute(const ofXml& xml, std::string name, bool required, T def) {
             std::string v = xml.getAttribute(name).getValue();
             if (v == "") {
                 if (required) {
                     throw std::runtime_error("Missing required attribute " + name + " in <" + xml.getName() + ">");
                 }

                 return def;
             }

            try {
                return boost::lexical_cast<T>(v);
            } catch (const boost::bad_lexical_cast& e) {
                throw std::runtime_error("Invalid value for attribute " + name + " in <" + xml.getName() + ">");
            }
        }
    }
}
