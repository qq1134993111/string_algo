#ifndef STRING_ALGO_FINDER_HPP
#define STRING_ALGO_FINDER_HPP

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <functional>
#include <iterator>
#include <string>
#include <utility>
#include "config.hpp"
#include "constants.hpp"

namespace string_algo {

namespace detail {

template <typename T>
std::size_t distance(const T& first, const T& last) {
    return std::distance(first, last);
}

} // namespace detail

// ---- 查找器概念 ----
// 查找器是一个函数对象，用于在范围内查找子串。
// 它返回一对迭代器 (first, last) 界定匹配位置，
// 若未找到匹配则返回范围的结束位置对。

template <typename PredT, typename IterT, typename Range2T>
class first_finder {
public:
    using value_type = IterT;

private:
    PredT pred;
    Range2T search_range;

public:
    first_finder(const Range2T& r, const PredT& p) : pred(p), search_range(r) {}

    std::pair<IterT, IterT> operator()(
        const std::pair<IterT, IterT>& seq_range) const {
        
        if (seq_range.first == seq_range.second || search_range.empty()) {
            return {seq_range.second, seq_range.second};
        }

        // 使用简单查找
        auto first = seq_range.first;
        auto last = seq_range.second;
        auto sf = search_range.cbegin();
        auto sl = search_range.cend();
        
        // 查找逻辑
        while (first != last) {
            auto it = first;
            auto sit = sf;
            bool found = true;
            while (it != last && sit != sl) {
                if (!pred(*it, *sit)) {
                    found = false;
                    break;
                }
                ++it;
                ++sit;
            }
            if (found && sit == sl) {
                return {first, it};
            }
            ++first;
        }
        return {seq_range.second, seq_range.second};
    }
};

// 简单相等辅助函数（区分大小写）
template <typename CharT>
inline bool is_eq_impl(const CharT& a, const CharT& b) { return a == b; }

// 不区分大小写的相等辅助函数
namespace detail {
template <typename CharT>
bool is_ieq_impl(const CharT& a, const CharT& b) {
    return std::tolower(static_cast<unsigned char>(a)) == 
           std::tolower(static_cast<unsigned char>(b));
}
} // namespace detail

// ---- last_finder：查找最后一次匹配 ----
template <typename PredT, typename IterT, typename Range2T>
class last_finder {
public:
    using value_type = IterT;

private:
    PredT pred;
    Range2T search_range;

public:
    last_finder(const Range2T& r, const PredT& p) : pred(p), search_range(r) {}

    std::pair<IterT, IterT> operator()(
        const std::pair<IterT, IterT>& seq_range) const {
        
        if (seq_range.first == seq_range.second || search_range.empty()) {
            return {seq_range.second, seq_range.second};
        }

        auto sf = search_range.begin();
        auto sl = search_range.end();
        std::size_t len = std::distance(sf, sl);
        auto start = seq_range.first;
        auto end = seq_range.second;
        
        // 从末尾开始搜索
        auto it2 = end;
        while (it2 >= start && std::distance(start, it2) >= static_cast<std::ptrdiff_t>(len)) {
            auto s = it2;
            std::advance(s, -static_cast<std::ptrdiff_t>(len));
            auto sf_it = sf;
            bool found = true;
            for (auto p = s; p != it2; ++p, ++sf_it) {
                if (!pred(*p, *sf_it)) {
                    found = false;
                    break;
                }
            }
            if (found) return {s, it2};
            --it2;
        }
        return {end, end};
    }
};

// ---- nth_finder：查找第 n 次匹配（从 0 开始计数）----
template <typename PredT, typename IterT, typename Range2T>
class nth_finder {
    std::size_t nth;
    first_finder<PredT, IterT, Range2T> first;

public:
    using value_type = IterT;

    nth_finder(std::size_t n, const Range2T& r, const PredT& p) : nth(n), first(r, p) {}

    std::pair<IterT, IterT> operator()(
        const std::pair<IterT, IterT>& seq_range) const {
        
        auto pos = seq_range.first;
        for (std::size_t i = 0; i <= nth; ++i) {
            auto match = first({pos, seq_range.second});
            if (match.first == seq_range.second) {
                return match;
            }
            if (i == nth) return match;
            pos = match.second;
        }
        return {seq_range.second, seq_range.second};
    }
};

// ---- head_finder：获取范围头部 ----
template <typename RangeT>
class head_finder {
    std::ptrdiff_t idx;
    using value_type = typename RangeT::value_type;

public:
    explicit head_finder(std::ptrdiff_t i) : idx(i) {}

    std::pair<value_type, value_type> operator()(
        const std::pair<value_type, value_type>& seq_range) const {
        
        auto first = seq_range.first;
        auto last = seq_range.second;
        
        if (idx < 0) {
            // 负索引：从末尾开始
            if (static_cast<std::size_t>(-idx) >= std::distance(first, last)) {
                return {last, last};
            }
            return {last, last};
        }

        auto it = first;
        if (static_cast<std::size_t>(idx) >= std::distance(first, last)) {
            return {last, last};
        }
        std::advance(it, idx);
        return {first, it};
    }
};

// ---- tail_finder：获取范围尾部 ----
template <typename RangeT>
class tail_finder {
    std::ptrdiff_t idx;
    using value_type = typename RangeT::value_type;

public:
    explicit tail_finder(std::ptrdiff_t i) : idx(i) {}

    std::pair<value_type, value_type> operator()(
        const std::pair<value_type, value_type>& seq_range) const {
        
        auto first = seq_range.first;
        auto last = seq_range.second;
        auto it = first;
        
        if (idx < 0) {
            if (static_cast<std::size_t>(-idx) >= std::distance(first, last)) {
                return {first, last};
            }
            return {first, last};
        }

        if (static_cast<std::size_t>(idx) >= std::distance(first, last)) {
            return {first, last};
        }
        std::advance(it, idx);
        return {it, last};
    }
};

// ---- token_finder：查找由谓词界定的第一个标记 ----
template <typename PredT, typename RangeT>
class token_finder {
    PredT is_begin;
    PredT is_end;
    using value_type = typename RangeT::value_type;

public:
    token_finder(const PredT& p1, const PredT& p2) : is_begin(p1), is_end(p2) {}

    std::pair<value_type, value_type> operator()(
        const std::pair<value_type, value_type>& seq_range) const {
        
        auto first = seq_range.first;
        auto last = seq_range.second;
        auto it = first;
        
        // 跳过前导分隔符
        while (it != last && is_begin(*it)) ++it;
        
        auto tok_start = it;
        while (it != last && !is_end(*it)) ++it;
        
        if (tok_start == it) {
            // 未找到标记
            return {last, last};
        }
        return {tok_start, it};
    }
};

// ---- range_finder：始终返回给定范围 ----
template <typename RangeT>
class range_finder {
    using value_type = typename RangeT::value_type;

public:
    std::pair<value_type, value_type> operator()(
        const std::pair<value_type, value_type>& seq_range) const {
        return seq_range;
    }
};

// ---- 便捷构造函数 ----
// 根据搜索范围自动推导迭代器类型
namespace detail {

// 根据搜索范围推导迭代器类型
template <typename Range2T>
using iter_of_search = decltype(std::declval<Range2T&>().begin());

// 根据搜索范围推导字符类型
template <typename Range2T>
using char_of_search = typename std::iterator_traits<iter_of_search<Range2T>>::value_type;

// 相等谓词工厂
template <typename CharT>
inline std::function<bool(CharT, CharT)> make_eq_pred(bool case_insensitive) {
    if (case_insensitive) {
        return [](CharT a, CharT b) {
            return std::tolower(static_cast<unsigned char>(a)) ==
                   std::tolower(static_cast<unsigned char>(b));
        };
    }
    return [](CharT a, CharT b) { return a == b; };
}

} // namespace detail

// first_finder 便捷构造函数
template <typename Range2T>
inline auto make_first_finder(const Range2T& search_range, bool case_insensitive = false) {
    using iter_t = detail::iter_of_search<Range2T>;
    using char_t = detail::char_of_search<Range2T>;
    return first_finder<std::function<bool(char_t, char_t)>, iter_t, Range2T>(
        search_range, detail::make_eq_pred<char_t>(case_insensitive));
}

// last_finder 便捷构造函数
template <typename Range2T>
inline auto make_last_finder(const Range2T& search_range, bool case_insensitive = false) {
    using iter_t = detail::iter_of_search<Range2T>;
    using char_t = detail::char_of_search<Range2T>;
    return last_finder<std::function<bool(char_t, char_t)>, iter_t, Range2T>(
        search_range, detail::make_eq_pred<char_t>(case_insensitive));
}

// nth_finder 便捷构造函数
template <typename Range2T>
inline auto make_nth_finder(std::size_t n, const Range2T& search_range, bool case_insensitive = false) {
    using iter_t = detail::iter_of_search<Range2T>;
    using char_t = detail::char_of_search<Range2T>;
    return nth_finder<std::function<bool(char_t, char_t)>, iter_t, Range2T>(
        n, search_range, detail::make_eq_pred<char_t>(case_insensitive));
}

// ---- 格式化器概念 ----

// const_formatter：始终返回常量字符串（忽略输入）
template <typename RangeT>
class const_formatter {
    RangeT val;
public:
    explicit const_formatter(const RangeT& v) : val(v) {}
    RangeT operator()(const RangeT&) const { return val; }
    RangeT operator()() const { return val; }
};

// identity_formatter：原样返回输入
template <typename RangeT>
class identity_formatter {
public:
    RangeT operator()(const RangeT& r) const { return r; }
};

// empty_formatter：返回空字符串
template <typename RangeT>
class empty_formatter {
public:
    RangeT operator()() const { return RangeT(); }
};

} // namespace string_algo

#endif // STRING_ALGO_FINDER_HPP
