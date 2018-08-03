#include "syntheffect/param/Params.h"

#include <map>

#include "ofMath.h"
#include "ofUtils.h"
#include "ofLog.h"

namespace syntheffect {
    namespace param {
        Params::Params() :
            float_params(),
            int_params(),
            bool_params(),
            float_func_params(),
            int_func_params(),
            bool_func_params(),
            texture_func_params() {
        }

        void Params::setParamAxisNegative(std::string name, std::string which) {
            bool_func_params[name] = [this, which]() {
                return this->float_params[which] < 0;
            };
        }

        void Params::setParamAxisPositive(std::string name, std::string which) {
            bool_func_params[name] = [this, which]() {
                return this->float_params[which] > 0;
            };
        }

        void Params::setParamAxis(std::string name, std::string which, bool absolute, float low, float high) {
            float_func_params[name] = [this, name, which, low, high, absolute]() {
                float v = this->float_params[which];

                v = ofMap(v, -1, 1, low, high);
                if (absolute) {
                    v = fabs(v);
                }


                return v;
            };
        }

        void Params::setParamButton(std::string name, std::string which) {
            bool_func_params[name] = [this, which]() {
                return this->bool_params[which];
            };
        }

        void Params::setParamButtonTime(std::string name, std::string which, float offset) {
            float_func_params[name] = [this, which, offset]() {
                return ofGetElapsedTimef() - this->float_params[which + "_pressed_at"] + offset;
            };
        }

        void Params::setParamWaveCos(std::string name, float shift, float freq, float low, float high) {
            float_func_params[name] = [shift, freq, low, high]() {
                float v = cos(ofGetElapsedTimef() * freq + shift);
                return ofMap(v, -1, 1, low, high);
            };
        }

        void Params::setParamWaveSin(std::string name, float shift, float freq, float low, float high) {
            float_func_params[name] = [shift, freq, low, high]() {
                float v = sin(ofGetElapsedTimef() * freq + shift);
                return ofMap(v, -1, 1, low, high);
            };
        }

        void Params::setParamWavePerlin(std::string name, float shift, float freq, float low, float high) {
            float_func_params[name] = [shift, freq, low, high]() {
                float v = ofNoise(ofGetElapsedTimef() * freq + shift);
                return ofMap(v, 0, 1, low, high);
            };
        }

        void Params::set(std::shared_ptr<Params> p) {
            for (const auto& kv : p->float_params) {
                float_params[kv.first] = kv.second;
            }

            for (const auto& kv : p->int_params) {
                int_params[kv.first] = kv.second;
            }

            for (const auto& kv : p->bool_params) {
                bool_params[kv.first] = kv.second;
            }

            for (const auto& kv : p->float_func_params) {
                float_func_params[kv.first] = kv.second;
            }

            for (const auto& kv : p->int_func_params) {
                int_func_params[kv.first] = kv.second;
            }

            for (const auto& kv : p->bool_func_params) {
                bool_func_params[kv.first] = kv.second;
            }

            for (const auto& kv : p->texture_func_params) {
                texture_func_params[kv.first] = kv.second;
            }
        }
    }
}
