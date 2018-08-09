#include "syntheffect/param/Params.h"

#include <map>
#include <math.h>

#include "ofMath.h"
#include "ofUtils.h"
#include "ofLog.h"

#define MAX_PARAMS_DEEP 1

namespace syntheffect {
    namespace param {
        void Params::set(const settings::ParamSettings p) {
            //ofLogNotice("Params", "name=%s value=%f variable_value=%s low=%f high=%f", p.name.c_str(), p.value, p.variable_value.c_str(), p.low, p.high);
            params_[p.name] = p;
        }

        settings::ParamSettings Params::resolveVariable(const settings::ParamSettings& original_p) {
            settings::ParamSettings p = original_p;

            for (int i=MAX_PARAMS_DEEP; i >= 0; i--) {
                if (p.variable_value == "") {
                    return p;
                }

                p = params_[p.variable_value];
            }

            throw std::runtime_error(
                    "Maximum depth of " +
                    std::to_string(MAX_PARAMS_DEEP) +
                    " reached while resolving param " +
                    original_p.variable_value);
        }


        float Params::resolveValue(const settings::ParamSettings& p) {
            settings::ParamSettings resolved_p = resolveVariable(p);
            float v = p.value;
            if (p.isVariable()) {
                v = resolveValue(resolved_p);
                v = ofMap(v, resolved_p.low, resolved_p.high, p.low, p.high);
            }

            v = (v * p.freq) + p.shift;

            if (p.func == settings::IdentityFunc) {
                return v;
            }

            if (p.func == settings::NoiseFunc) {
                return ofMap(ofNoise(v), 0, 1, p.low, p.high);
            }

            if (p.func == settings::SinFunc) {
                return ofMap(sin(v), -1, 1, p.low, p.high);
            }

            if (p.func == settings::CosFunc) {
                return ofMap(cos(v), -1, 1, p.low, p.high);
            }

            throw std::runtime_error("Unsupported parameter func type");
        }

        bool Params::getBool(std::string name) {
            settings::ParamSettings p = params_.at(name);
            float v = resolveValue(p);

            // Inherit casting if not specified
            settings::ParamCast cast = p.cast;
            if (p.isVariable() && p.cast == settings::NoCast) {
                cast = resolveVariable(p).cast;
            }

            if (cast == settings::NegativeIsTrueCast) {
                return v < 0;
            } else if (cast == settings::PositiveIsTrueCast) {
                return v > 0;
            } else {
                throw std::runtime_error("bool not appropriate for given cast for parameter " + p.name);
            }
        }

        int Params::getInt(std::string name) {
            settings::ParamSettings p = params_.at(name);
            float v = resolveValue(p);

            settings::ParamCast cast = p.cast;
            if (p.isVariable() && p.cast == settings::NoCast) {
                cast = params_.at(p.variable_value).cast;
            }

            if (cast == settings::RoundIntCast) {
                return round(v);
            } else {
                throw std::runtime_error("int not appropriate for given cast for parameter " + p.name);
            }
        }

        float Params::getFloat(std::string name) {
            settings::ParamSettings p = params_.at(name);
            float v = resolveValue(p);

            //ofLogNotice("Params", "name=%s value=%f variable_value=%s low=%f high=%f", p.name.c_str(), p.value, p.variable_value.c_str(), p.low, p.high);

            if (p.cast == settings::NoCast) {
                return v;
            } else {
                throw std::runtime_error("float not appropriate for given cast for parameter " + p.name);
            }
        }

        ofTexture Params::getTexture(std::string name) {
            if (texture_params_.count(name) > 0) {
                name = texture_params_[name];
            }

            return textures_[name]();
        }

        bool Params::exists(std::string name) {
            return params_.count(name) > 0 || textures_.count(name) > 0 || texture_params_.count(name) > 0;
        }

        void Params::setTexture(std::string name, std::function<ofTexture()> v) {
            textures_[name] = v;
        }

        void Params::setTexture(std::string name, std::string target) {
            texture_params_[name] = target;
        }

        bool Params::isFloat(const settings::ParamSettings& p) {
            return (p.cast == settings::NoCast) ||
                (p.cast == settings::NoCast && p.isVariable() && isFloat(resolveVariable(p)));
        }

        bool Params::isBool(const settings::ParamSettings& p) {
            return (p.cast == settings::NegativeIsTrueCast || p.cast == settings::PositiveIsTrueCast) ||
                (p.cast == settings::NoCast && p.isVariable() && isBool(resolveVariable(p)));
        }

        bool Params::isInt(const settings::ParamSettings& p) {
            return (p.cast == settings::RoundIntCast) ||
                (p.cast == settings::NoCast && p.isVariable() && isInt(resolveVariable(p)));
        }

        std::map<std::string, bool> Params::getBools() {
            std::map<std::string, bool> bools;
            for (auto const& kv : params_) {
                auto p = kv.second;
                if (isBool(p)) {
                    bools[kv.first] = getBool(kv.first);
                }
            }

            return bools;
        }

        std::map<std::string, int> Params::getInts() {
            std::map<std::string, int> ints;
            for (auto const& kv : params_) {
                auto p = kv.second;
                if (isInt(p)) {
                    ints[kv.first] = getInt(kv.first);
                }
            }

            return ints;
        }

        std::map<std::string, float> Params::getFloats() {
            std::map<std::string, float> floats;
            for (auto const& kv : params_) {
                auto p = kv.second;
                if (isFloat(p)) {
                    floats[kv.first] = getFloat(kv.first);
                }
            }

            return floats;
        }

        std::map<std::string, ofTexture> Params::getTextures() {
            std::map<std::string, ofTexture> textures;
            for (auto const& kv : texture_params_) {
                textures[kv.first] = getTexture(kv.first);
            }

            return textures;
        }

        void Params::copyTo(Params& p) {
            for (const auto kv : params_) {
                p.set(kv.second);
            }
        }
    }
}

#undef MAX_PARAMS_DEEP
