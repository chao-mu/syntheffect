#pragma once

#include <vector>
#include <map>
#include <stdexcept>

namespace syntheffect {
    namespace safety {
        template <typename T> T& at(const std::vector<T>& vec, int i, std::string what) {
            if (vec.count(i) > 0) {
                return vec[i];
            }

            throw std::out_of_range("Unable to lookup index " + std::to_string(i) + " in " + what);
        }

        template <typename K, typename V> V& at(std::map<K, V> m, K k, std::string what) {
            if (m.count(k) > 0) {
                return m[k];
            }

            throw std::out_of_range("Unable to lookup key " + std::to_string(k) + " in " + what);
        }
    }
}
