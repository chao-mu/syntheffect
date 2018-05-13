#include "Params.h"

namespace syntheffect {
    namespace graphics {
        Params::Params() :
            float_params(), 
            int_params(),
            bool_params(),
            float_func_params(), 
            int_func_params(),
            bool_func_params(),
            texture_func_params() {
        }

        void Params::set(std::shared_ptr<graphics::Params> p) {
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
