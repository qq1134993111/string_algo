#ifndef STRING_ALGO_TRIM_HPP
#define STRING_ALGO_TRIM_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
#include "config.hpp"
#include "classification.hpp"

namespace string_algo {

namespace detail {

template <typename RangeT, typename PredT>
void trim_left_impl(RangeT& range, PredT pred) {
    auto first = range.begin();
    auto last = range.end();
    
    while (first != last && pred(*first)) {
        ++first;
    }
    
    range.erase(range.begin(), first);
}

template <typename RangeT, typename PredT>
void trim_right_impl(RangeT& range, PredT pred) {
    auto first = range.begin();
    auto last = range.end();
    
    while (last != first && pred(*(last - 1))) {
        --last;
    }
    
    range.erase(last, range.end());
}

} // namespace detail

// ---- 修剪算法 ----

template <typename RangeT, typename PredT>
void trim_left_if(RangeT& range, PredT pred) {
    detail::trim_left_impl(range, pred);
}

template <typename RangeT>
void trim_left(RangeT& range, const std::locale& loc = std::locale()) {
    auto pred = is_space<typename RangeT::value_type>(loc);
    trim_left_if(range, pred);
}

template <typename RangeT, typename PredT>
RangeT trim_left_copy_if(const RangeT& range, PredT pred) {
    auto first = range.begin();
    auto last = range.end();
    
    while (first != last && pred(*first)) {
        ++first;
    }
    
    return RangeT(first, last);
}

template <typename RangeT>
RangeT trim_left_copy(const RangeT& range, const std::locale& loc = std::locale()) {
    auto pred = is_space<typename RangeT::value_type>(loc);
    return trim_left_copy_if(range, pred);
}

template <typename RangeT, typename PredT>
void trim_right_if(RangeT& range, PredT pred) {
    detail::trim_right_impl(range, pred);
}

template <typename RangeT>
void trim_right(RangeT& range, const std::locale& loc = std::locale()) {
    auto pred = is_space<typename RangeT::value_type>(loc);
    trim_right_if(range, pred);
}

template <typename RangeT, typename PredT>
RangeT trim_right_copy_if(const RangeT& range, PredT pred) {
    auto first = range.begin();
    auto last = range.end();
    
    while (last != first && pred(*(last - 1))) {
        --last;
    }
    
    return RangeT(first, last);
}

template <typename RangeT>
RangeT trim_right_copy(const RangeT& range, const std::locale& loc = std::locale()) {
    auto pred = is_space<typename RangeT::value_type>(loc);
    return trim_right_copy_if(range, pred);
}

template <typename RangeT, typename PredT>
void trim_if(RangeT& range, PredT pred) {
    trim_left_if(range, pred);
    trim_right_if(range, pred);
}

template <typename RangeT>
void trim(RangeT& range, const std::locale& loc = std::locale()) {
    auto pred = is_space<typename RangeT::value_type>(loc);
    trim_if(range, pred);
}

template <typename RangeT, typename PredT>
RangeT trim_copy_if(const RangeT& range, PredT pred) {
    auto first = range.begin();
    auto last = range.end();
    
    while (first != last && pred(*first)) {
        ++first;
    }
    
    while (last != first && pred(*(last - 1))) {
        --last;
    }
    
    return RangeT(first, last);
}

template <typename RangeT>
RangeT trim_copy(const RangeT& range, const std::locale& loc = std::locale()) {
    auto pred = is_space<typename RangeT::value_type>(loc);
    return trim_copy_if(range, pred);
}

// ---- 修剪所有（压缩内部空白） ----
template <typename RangeT, typename PredT>
void trim_all_if(RangeT& range, PredT pred) {
    RangeT result;
    result.reserve(range.size());
    
    auto first = range.begin();
    auto last = range.end();
    bool in_space = false;
    
    // 跳过前导空格
    while (first != last && pred(*first)) {
        ++first;
    }
    
    for (; first != last; ++first) {
        if (pred(*first)) {
            if (!in_space) {
                result.push_back(' ');
                in_space = true;
            }
        } else {
            result.push_back(*first);
            in_space = false;
        }
    }
    
    // 移除结果末尾的空白
    trim_right_if(result, pred);
    
    range = std::move(result);
}

template <typename RangeT>
void trim_all(RangeT& range, const std::locale& loc = std::locale()) {
    auto pred = is_space<typename RangeT::value_type>(loc);
    trim_all_if(range, pred);
}

template <typename RangeT, typename PredT>
RangeT trim_all_copy_if(const RangeT& range, PredT pred) {
    RangeT result;
    result.reserve(range.size());
    
    auto first = range.begin();
    auto last = range.end();
    bool in_space = false;
    
    while (first != last && pred(*first)) {
        ++first;
    }
    
    for (; first != last; ++first) {
        if (pred(*first)) {
            if (!in_space) {
                result.push_back(' ');
                in_space = true;
            }
        } else {
            result.push_back(*first);
            in_space = false;
        }
    }
    
    // 移除结果末尾的空白
    trim_right_if(result, pred);
    
    return result;
}

template <typename RangeT>
RangeT trim_all_copy(const RangeT& range, const std::locale& loc = std::locale()) {
    auto pred = is_space<typename RangeT::value_type>(loc);
    return trim_all_copy_if(range, pred);
}

} // namespace string_algo

#endif // STRING_ALGO_TRIM_HPP
