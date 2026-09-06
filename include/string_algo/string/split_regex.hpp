#ifndef STRING_ALGO_SPLIT_REGEX_HPP
#define STRING_ALGO_SPLIT_REGEX_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <regex>
#include <string>
#include <utility>
#include <vector>
#include "config.hpp"
#include "constants.hpp"

namespace string_algo {

namespace detail {

template <typename RangeT>
void split_regex_impl(std::vector<RangeT>& result, RangeT& range, 
                      const std::regex& re) {
    using char_type = typename RangeT::value_type;
    
    // 转换为字符串以进行正则处理
    std::basic_string<char_type> str;
    for (auto it = range.begin(); it != range.end(); ++it) {
        str.push_back(*it);
    }
    
    result.clear();
    
    auto first = str.begin();
    auto last = str.end();
    
    while (first != last) {
        // 构造剩余子串进行查找（避免迭代器版本的兼容性问题）
        std::basic_string<char_type> remaining;
        for (auto it = first; it != last; ++it) remaining.push_back(*it);
        
        std::smatch match;
        if (std::regex_search(remaining, match, re)) {
            auto pos = match.position(0);
            auto len = match.length(0);
            // 添加匹配前的部分
            result.emplace_back(first, first + pos);
            first = first + pos + len;
        } else {
            // 没有更多匹配，添加剩余部分
            result.emplace_back(first, last);
            break;
        }
    }
    
    // 若字符串以分隔符结尾，移除末尾的空元素
    if (!result.empty() && result.back().empty()) {
        result.pop_back();
    }
}

} // namespace detail

// ---- 正则分割算法 ----

template <typename RangeT>
void split_regex(std::vector<RangeT>& result, RangeT& range, const std::regex& re) {
    detail::split_regex_impl(result, range, re);
}

template <typename RangeT>
void find_all_regex(std::vector<std::pair<
    typename RangeT::const_iterator, 
    typename RangeT::const_iterator>>& result, 
    RangeT& range, const std::regex& re) {
    using char_type = typename RangeT::value_type;
    
    std::basic_string<char_type> str;
    for (auto it = range.begin(); it != range.end(); ++it) {
        str.push_back(*it);
    }
    
    result.clear();
    
    auto first = range.begin();
    auto last = range.end();
    
    while (first != last) {
        // 构造剩余子串进行查找（避免迭代器版本的兼容性问题）
        std::basic_string<char_type> remaining;
        for (auto it = first; it != last; ++it) remaining.push_back(*it);
        
        std::smatch match;
        if (std::regex_search(remaining, match, re)) {
            auto pos = match.position(0);
            auto len = match.length(0);
            // 记录匹配的迭代器对
            auto match_first = first + pos;
            auto match_last = match_first + len;
            result.emplace_back(match_first, match_last);
            first = match_last;
        } else {
            break;
        }
    }
}

} // namespace string_algo

#endif // STRING_ALGO_SPLIT_REGEX_HPP
