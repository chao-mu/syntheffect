#include "syntheffect/param/Params.h"

#include <map>
#include <math.h>

#include "ofMath.h"
#include "ofUtils.h"
#include "ofLog.h"

#define MAX_PARAMS_DEEP 100

namespace syntheffect {
    namespace param {
        void Params::set(Param p) {
            params_[p.name] = p;
        }

        const Param& Params::at(std::string name) const {
            if (params_.count(name) == 0) {
                throw std::out_of_range("Parameter '" + name + "' not set, but attempted to be retrieved");
            }

            return params_.at(name);
        }

        const ParamCast& Params::resolveCast(const Param& p, int resolutions) const {
            if (p.isVariable() && resolutions > MAX_PARAMS_DEEP) {
                throw std::runtime_error("loop involving parameter '" + p.variable_value  + "' detected");
            }

            if (p.cast != NoCast || !p.isVariable()) {
                return p.cast;
            }

            return resolveCast(resolveParent(p), resolutions + 1);
        }

        const Param& Params::resolveParent(const Param& p) const {
            if (params_.count(p.variable_value) == 0) {
                throw std::out_of_range("Parameter '" + p.variable_value + "' not set, required by parameter '" + p.name + "'");
            }

            return at(p.variable_value);
        }


        float Params::resolveValue(const Param& p, int resolutions) const {
            if (p.isVariable() && resolutions > MAX_PARAMS_DEEP) {
                throw std::runtime_error("loop involving  parameter" + p.variable_value  + " detected");
            }

            float v = p.value;

            if (p.isVariable()) {
                const Param& resolved_p = resolveParent(p);
                v = resolveValue(resolved_p, resolutions + 1);
                if (resolved_p.limits.exists && p.limits.exists && p.func == IdentityFunc) {
                    return ofMap(v, resolved_p.limits.value.low, resolved_p.limits.value.high, p.limits.value.low, p.limits.value.high);
                }
            }

            v = (v * p.freq) + p.shift;

            if (p.func == IdentityFunc) {
                return v;
            }

            float from_low;
            float from_high;
            if (p.func == NoiseFunc) {
                v = ofNoise(v);
                from_low = 0;
                from_high = 1;
            } else if (p.func == SinFunc) {
                from_low = -1;
                from_high = 1;
                v = sin(v);
            } else if (p.func == CosFunc) {
                from_low = -1;
                from_high = 1;
                v = cos(v);
            } else {
                throw std::runtime_error("Unsupported parameter func type");
            }

            if (p.limits.exists) {
                v = ofMap(v, from_low, from_high, p.limits.value.low, p.limits.value.high);
            }

            return v;
        }

        bool Params::getBool(std::string name) const {
            Param p = at(name);

            float v = resolveValue(p);
            ParamCast cast = resolveCast(p);

            if (cast == NegativeIsTrueCast) {
                return v < 0;
            } else if (cast == PositiveIsTrueCast) {
                return v > 0;
            } else {
                throw std::runtime_error("bool not appropriate for given cast for parameter " + p.name);
            }
        }

        int Params::getInt(std::string name) const {
            Param p = at(name);

            float v = resolveValue(p);
            ParamCast cast = resolveCast(p);
            if (cast == RoundIntCast) {
                return round(v);
            } else {
                throw std::runtime_error("int not appropriate for given cast for parameter " + p.name);
            }
        }

        float Params::getFloat(std::string name) const {
            Param p = at(name);

            float v = resolveValue(p);
            ParamCast cast = resolveCast(p);

            if (cast == NoCast) {
                return v;
            } else {
                throw std::runtime_error("float not appropriate for given cast for parameter " + p.name);
            }
        }

        ofTexture Params::getTexture(std::string name) const {
            if (texture_params_.count(name) > 0) {
                name = texture_params_.at(name);
            }

            return textures_.at(name)();
        }

        bool Params::exists(std::string name) const {
            return params_.count(name) > 0 || textures_.count(name) > 0 || texture_params_.count(name) > 0;
        }

        void Params::setTexture(std::string name, std::function<ofTexture()> v) {
            textures_[name] = v;
        }

        void Params::setTexture(std::string name, std::string target) {
            texture_params_[name] = target;
        }

        bool Params::isFloat(const Param& p) const {
            ParamCast cast = resolveCast(p);
            return cast == NoCast;
        }

        bool Params::isBool(const Param& p) const {
            ParamCast cast = resolveCast(p);
            return cast == NegativeIsTrueCast || cast == PositiveIsTrueCast;
        }

        bool Params::isInt(const Param& p) const {
            ParamCast cast = resolveCast(p);
            return cast == RoundIntCast;
        }

        std::map<std::string, bool> Params::getBools() const {
            std::map<std::string, bool> bools;
            for (auto const& kv : params_) {
                auto p = kv.second;
                if (isBool(p)) {
                    bools[kv.first] = getBool(kv.first);
                }
            }

            return bools;
        }

        std::map<std::string, int> Params::getInts() const {
            std::map<std::string, int> ints;
            for (auto const& kv : params_) {
                auto p = kv.second;
                if (isInt(p)) {
                    ints[kv.first] = getInt(kv.first);
                }
            }

            return ints;
        }

        std::map<std::string, float> Params::getFloats() const {
            std::map<std::string, float> floats;
            for (auto const& kv : params_) {
                auto p = kv.second;
                if (isFloat(p)) {
                    floats[kv.first] = getFloat(kv.first);
                }
            }

            return floats;
        }

        std::map<std::string, ofTexture> Params::getTextures() const {
            std::map<std::string, ofTexture> textures;
            for (auto const& kv : texture_params_) {
                textures[kv.first] = getTexture(kv.first);
            }

            return textures;
        }

        void Params::copyTo(Params& p) const {
            for (const auto kv : params_) {
                p.set(kv.second);
            }

            for (const auto& kv : textures_) {
                p.setTexture(kv.first, kv.second);
            }
        }
    }
}

#undef MAX_PARAMS_DEEP
