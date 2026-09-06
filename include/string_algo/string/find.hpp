#ifndef STRING_ALGO_FIND_HPP
#define STRING_ALGO_FIND_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include "config.hpp"
#include "finder.hpp"

namespace string_algo {

// ---- 查找算法 ----

// 使用 std::string::find 的简化版本

template <typename RangeT, typename SubT>
bool find_first(const RangeT& range, const SubT& sub) {
    if (sub.empty()) return true;
    
    auto first = range.begin();
    auto last = range.end();
    
    while (first != last) {
        auto it = first;
        auto sub_it = sub.begin();
        bool found = true;
        while (it != last && sub_it != sub.end()) {
            if (*it != *sub_it) {
                found = false;
                break;
            }
            ++it;
            ++sub_it;
        }
        if (found && sub_it == sub.end()) {
            return true;
        }
        ++first;
    }
    return false;
}

// 使用 find_iterator 的通用查找
template <typename FinderT, typename RangeT>
auto find(const FinderT& finder, RangeT& range) 
    -> find_iterator<FinderT, decltype(range.begin()), RangeT> {
    using iter_t = decltype(range.begin());
    return find_iterator<FinderT, iter_t, RangeT>(finder, range.begin(), range.end(), range);
}

template <typename RangeT, typename SubT>
bool ifind_first(const RangeT& range, const SubT& sub) {
    if (sub.empty()) return true;
    
    auto first = range.begin();
    auto last = range.end();
    
    while (first != last) {
        auto it = first;
        auto sub_it = sub.begin();
        bool found = true;
        while (it != last && sub_it != sub.end()) {
            auto a = std::tolower(static_cast<unsigned char>(*it));
            auto b = std::tolower(static_cast<unsigned char>(*sub_it));
            if (a != b) {
                found = false;
                break;
            }
            ++it;
            ++sub_it;
        }
        if (found && sub_it == sub.end()) {
            return true;
        }
        ++first;
    }
    return false;
}

} // namespace string_algo

#endif // STRING_ALGO_FIND_HPP
