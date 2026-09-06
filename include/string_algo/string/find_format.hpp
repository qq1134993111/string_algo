#ifndef STRING_ALGO_FIND_FORMAT_HPP
#define STRING_ALGO_FIND_FORMAT_HPP

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
#include "config.hpp"
#include "find_iterator.hpp"

namespace string_algo {

// ---- find_format：通用替换算法 ----
// 
// 对查找器找到的每个匹配应用格式化器，
// 并将格式化结果通过回调传出。

namespace detail {

template <typename FinderT, typename RangeT, typename FormatterT, typename CallbackT>
void find_format_impl(FinderT finder, RangeT& range, FormatterT formatter, 
                      CallbackT callback) {
    
    using iter_t = decltype(range.begin());
    using char_type = typename RangeT::value_type;
    
    find_iterator<FinderT, iter_t, RangeT> fit(
        finder, range.begin(), range.end(), range);
    
    auto last = range.begin();
    
    for (; fit != find_iterator<FinderT, iter_t, RangeT>(); ++fit) {
        auto first = fit.begin();
        auto second = fit.end();
        if (first == second) break;
        
        // 处理匹配前的间隔
        RangeT gap;
        for (auto it = last; it != first; ++it) {
            gap.push_back(*it);
        }
        callback(gap);
        
        // 处理匹配——应用格式化器
        RangeT match;
        for (auto it = first; it != second; ++it) {
            match.push_back(*it);
        }
        RangeT formatted;
        for (auto ch : formatter(match)) {
            formatted.push_back(ch);
        }
        callback(formatted);
        
        last = second;
    }
    
    // 处理尾部
    RangeT tail;
    for (auto it = last; it != range.end(); ++it) {
        tail.push_back(*it);
    }
    callback(tail);
}

} // namespace detail

// ---- find_format ----
template <typename FinderT, typename RangeT, typename FormatterT, typename CallbackT>
void find_format(FinderT finder, RangeT& range, FormatterT formatter, 
                 CallbackT callback) {
    detail::find_format_impl(finder, range, formatter, callback);
}

// ---- find_format_copy ----
template <typename FinderT, typename RangeT, typename FormatterT>
RangeT find_format_copy(FinderT finder, const RangeT& range, FormatterT formatter) {
    // 创建 range 的副本，因为 find_format_impl 需要非 const 引用
    RangeT range_copy(range);
    RangeT result;
    result.reserve(range.size());
    
    find_format(finder, range_copy, formatter, [&result](const RangeT& part) {
        for (auto ch : part) {
            result.push_back(ch);
        }
    });
    
    return result;
}

// ---- find_format_all ----
template <typename FinderT, typename RangeT, typename FormatterT, typename CallbackT>
void find_format_all(FinderT finder, RangeT& range, FormatterT formatter, 
                     CallbackT callback) {
    // 与 find_format 类似，但会在替换后继续
    detail::find_format_impl(finder, range, formatter, callback);
}

// ---- find_format_all_copy ----
template <typename FinderT, typename RangeT, typename FormatterT>
RangeT find_format_all_copy(FinderT finder, const RangeT& range, FormatterT formatter) {
    // 创建 range 的副本
    RangeT range_copy(range);
    RangeT result;
    result.reserve(range.size());
    
    find_format_all(finder, range_copy, formatter, [&result](const RangeT& part) {
        for (auto ch : part) {
            result.push_back(ch);
        }
    });
    
    return result;
}

} // namespace string_algo

#endif // STRING_ALGO_FIND_FORMAT_HPP
