#ifndef STRING_ALGO_REPLACE_REGEX_HPP
#define STRING_ALGO_REPLACE_REGEX_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <regex>
#include <string>
#include <utility>
#include <vector>
#include "config.hpp"

namespace string_algo {

namespace detail {

template <typename RangeT, typename FormatterT>
void replace_regex_impl(RangeT& range, const std::regex& re, 
                        const std::string& fmt, const FormatterT& formatter) {
    using char_type = typename RangeT::value_type;
    
    // 转换为字符串以进行正则处理
    std::basic_string<char_type> str;
    for (auto it = range.begin(); it != range.end(); ++it) {
        str.push_back(*it);
    }
    
    // 使用带格式字符串的 std::regex_replace
    // 注意：std::regex_replace 的格式语法（$1、$2 等）与 Boost 类似
    auto result = std::regex_replace(str, re, fmt);
    
    // 复制回原范围
    range.erase(range.begin(), range.end());
    for (auto ch : result) {
        range.push_back(ch);
    }
}

} // namespace detail

// ---- 正则替换算法 ----

template <typename RangeT>
void replace_regex(RangeT& range, const std::regex& re, const std::string& fmt) {
    detail::replace_regex_impl(range, re, fmt, [](){});
}

template <typename RangeT>
RangeT replace_regex_copy(const RangeT& range, const std::regex& re, const std::string& fmt) {
    RangeT result;
    result.reserve(range.size());
    for (auto it = range.begin(); it != range.end(); ++it) {
        result.push_back(*it);
    }
    replace_regex(result, re, fmt);
    return result;
}

template <typename RangeT>
void erase_regex(RangeT& range, const std::regex& re) {
    replace_regex(range, re, "");
}

template <typename RangeT>
RangeT erase_regex_copy(const RangeT& range, const std::regex& re) {
    RangeT result;
    result.reserve(range.size());
    for (auto it = range.begin(); it != range.end(); ++it) {
        result.push_back(*it);
    }
    erase_regex(result, re);
    return result;
}

} // namespace string_algo

#endif // STRING_ALGO_REPLACE_REGEX_HPP
