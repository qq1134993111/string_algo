#ifndef STRING_ALGO_JOIN_HPP
#define STRING_ALGO_JOIN_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include "config.hpp"

namespace string_algo {

// ---- 连接算法 ----

template <typename Range1T, typename Range2T>
Range2T join(const Range1T& r1, const Range2T& r2) {
    Range2T result;
    
    auto it1 = r1.begin();
    auto it1_end = r1.end();
    
    while (it1 != it1_end) {
        auto it2 = it1->begin();
        auto it2_end = it1->end();
        
        while (it2 != it2_end) {
            result.push_back(*it2);
            ++it2;
        }
        ++it1;
    }
    
    return result;
}

template <typename Range1T, typename Range2T>
Range2T join_if(const Range1T& r1, const Range2T& r2, auto pred) {
    Range2T result;
    
    auto it1 = r1.begin();
    auto it1_end = r1.end();
    
    while (it1 != it1_end) {
        if (pred(*it1)) {
            auto it2 = it1->begin();
            auto it2_end = it1->end();
            
            while (it2 != it2_end) {
                result.push_back(*it2);
                ++it2;
            }
        }
        ++it1;
    }
    
    return result;
}

} // namespace string_algo

#endif // STRING_ALGO_JOIN_HPP
