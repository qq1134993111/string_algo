#ifndef STRING_ALGO_FIND_REGEX_HPP
#define STRING_ALGO_FIND_REGEX_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <regex>
#include <string>
#include <utility>
#include <vector>
#include "config.hpp"
#include "find_iterator.hpp"

namespace string_algo {

// ---- 基于正则的查找 ----

// regex_finder 前向声明
template <typename IterT>
class regex_finder;

// 使用标准 regex 进行查找
template <typename RangeT>
auto find_regex(const std::regex& re, RangeT& range) 
    -> find_iterator<regex_finder<typename RangeT::const_iterator>, 
                    typename RangeT::const_iterator, RangeT> {
    
    using iter_t = typename RangeT::const_iterator;
    using char_type = typename RangeT::value_type;
    
    // 将范围转换为字符串以供正则匹配
    std::basic_string<char_type> str;
    for (auto it = range.begin(); it != range.end(); ++it) {
        str.push_back(*it);
    }
    
    // 等价于使用 std::regex_iterator
    std::smatch first_match;
    std::regex_search(str, first_match, re);
    
    // 使用自定义查找器构建 find_iterator
    auto finder = regex_finder<iter_t>(re);
    return find_iterator<decltype(finder), iter_t, RangeT>(
        finder, range.begin(), range.end(), range);
}

// 正则查找器
template <typename IterT>
class regex_finder {
    std::regex re;
    std::string buffer;
public:
    using value_type = typename std::iterator_traits<IterT>::value_type;

    explicit regex_finder(const std::regex& r) : re(r) {}

    std::pair<IterT, IterT> operator()(std::pair<IterT, IterT> seq_range) const {
        // 从范围构建字符串
        std::basic_string<value_type> sub;
        for (auto it = seq_range.first; it != seq_range.second; ++it) {
            sub.push_back(*it);
        }
        
        // 使用 std::regex_search 搜索
        std::smatch match;
        std::size_t pos = 0;
        while (pos < sub.size()) {
            if (std::regex_search(sub.begin() + pos, sub.end(), match, re)) {
                IterT first = seq_range.first;
                IterT last = first;
                std::advance(first, static_cast<std::ptrdiff_t>(match.position(0) + pos));
                std::advance(last, static_cast<std::ptrdiff_t>(match.position(0) + pos + match.length(0)));
                return {first, last};
            }
            // 将 pos 递增 1（regex_search 不返回位置，因此使用一种变通方法）
            pos++;
        }
        return {seq_range.second, seq_range.second};
    }
};

// ---- 便捷封装 ----

template <typename RangeT>
bool find_regex(const RangeT& range, const std::regex& re) {
    using char_type = typename RangeT::value_type;
    
    std::basic_string<char_type> str;
    for (auto it = range.begin(); it != range.end(); ++it) {
        str.push_back(*it);
    }
    
    return std::regex_search(str, re);
}

} // namespace string_algo

#endif // STRING_ALGO_FIND_REGEX_HPP
