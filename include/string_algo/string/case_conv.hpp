#ifndef STRING_ALGO_CASE_CONV_HPP
#define STRING_ALGO_CASE_CONV_HPP

#include <algorithm>
#include <cctype>
#include <locale>
#include <string>
#include "config.hpp"

namespace string_algo {

// ---- 大小写转换 ----

template <typename RangeT>
void to_upper(RangeT& str, const std::locale& loc = std::locale()) {
    auto& ct = std::use_facet<std::ctype<typename RangeT::value_type>>(loc);
    for (auto& ch : str) {
        ch = ct.toupper(ch);
    }
}

template <typename RangeT>
RangeT to_upper_copy(const RangeT& str, const std::locale& loc = std::locale()) {
    RangeT result;
    result.reserve(str.size());
    auto& ct = std::use_facet<std::ctype<typename RangeT::value_type>>(loc);
    for (auto ch : str) {
        result.push_back(ct.toupper(ch));
    }
    return result;
}

template <typename RangeT>
void to_lower(RangeT& str, const std::locale& loc = std::locale()) {
    auto& ct = std::use_facet<std::ctype<typename RangeT::value_type>>(loc);
    for (auto& ch : str) {
        ch = ct.tolower(ch);
    }
}

template <typename RangeT>
RangeT to_lower_copy(const RangeT& str, const std::locale& loc = std::locale()) {
    RangeT result;
    result.reserve(str.size());
    auto& ct = std::use_facet<std::ctype<typename RangeT::value_type>>(loc);
    for (auto ch : str) {
        result.push_back(ct.tolower(ch));
    }
    return result;
}

} // namespace string_algo

#endif // STRING_ALGO_CASE_CONV_HPP
