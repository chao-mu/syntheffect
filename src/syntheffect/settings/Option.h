#pragma once

namespace syntheffect {
    namespace settings {
        template<typename T>
        struct Option {
            T value;
            bool exists = false;

            static Option<T> make() {
                Option<T> opt;
                opt.exists = false;
                return opt;
            }

            static Option<T> make(T value) {
                Option<T> opt;
                opt.exists = true;
                opt.value = value;
                return opt;
            }
        };
    }
}
