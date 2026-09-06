#ifndef STRING_ALGO_CLASSIFICATION_HPP
#define STRING_ALGO_CLASSIFICATION_HPP

#include <cctype>
#include <functional>
#include <iterator>
#include <locale>
#include <string>
#include <vector>
#include "config.hpp"

namespace string_algo {

// ---- 字符分类谓词 ----

// 基于 ctype 掩码的通用分类
template <typename CharT>
class is_classified {
    const std::ctype<CharT>& ct;
    const typename std::ctype<CharT>::mask mask;
public:
    is_classified(const std::ctype<CharT>& ctype, typename std::ctype<CharT>::mask m)
        : ct(ctype), mask(m) {}
    
    bool operator()(CharT c) const {
        return ct.is(mask, c);
    }
};

// 工厂函数：根据 locale 和掩码创建分类器
template <typename CharT, typename MaskT>
inline auto make_is_classified(const std::locale& loc, MaskT mask) 
    -> is_classified<CharT> {
    return is_classified<CharT>(
        std::use_facet<std::ctype<CharT>>(loc), static_cast<typename std::ctype<CharT>::mask>(mask));
}

// 工厂函数：根据 ctype 和掩码创建分类器
template <typename CharT>
inline auto make_is_classified(const std::ctype<CharT>& ctype, typename std::ctype<CharT>::mask mask) 
    -> is_classified<CharT> {
    return is_classified<CharT>(ctype, mask);
}

// 使用 std::ctype 掩码的便捷谓词
template <typename CharT = char>
inline auto is_space(const std::locale& loc = std::locale()) 
    -> is_classified<CharT> {
    return make_is_classified<CharT>(loc, std::ctype_base::space);
}

template <typename CharT = char>
inline auto is_alnum(const std::locale& loc = std::locale()) 
    -> is_classified<CharT> {
    return make_is_classified<CharT>(loc, std::ctype_base::alnum);
}

template <typename CharT = char>
inline auto is_alpha(const std::locale& loc = std::locale()) 
    -> is_classified<CharT> {
    return make_is_classified<CharT>(loc, std::ctype_base::alpha);
}

template <typename CharT = char>
inline auto is_cntrl(const std::locale& loc = std::locale()) 
    -> is_classified<CharT> {
    return make_is_classified<CharT>(loc, std::ctype_base::cntrl);
}

template <typename CharT = char>
inline auto is_digit(const std::locale& loc = std::locale()) 
    -> is_classified<CharT> {
    return make_is_classified<CharT>(loc, std::ctype_base::digit);
}

template <typename CharT = char>
inline auto is_graph(const std::locale& loc = std::locale()) 
    -> is_classified<CharT> {
    return make_is_classified<CharT>(loc, std::ctype_base::graph);
}

template <typename CharT = char>
inline auto is_lower(const std::locale& loc = std::locale()) 
    -> is_classified<CharT> {
    return make_is_classified<CharT>(loc, std::ctype_base::lower);
}

template <typename CharT = char>
inline auto is_print(const std::locale& loc = std::locale()) 
    -> is_classified<CharT> {
    return make_is_classified<CharT>(loc, std::ctype_base::print);
}

template <typename CharT = char>
inline auto is_punct(const std::locale& loc = std::locale()) 
    -> is_classified<CharT> {
    return make_is_classified<CharT>(loc, std::ctype_base::punct);
}

template <typename CharT = char>
inline auto is_upper(const std::locale& loc = std::locale()) 
    -> is_classified<CharT> {
    return make_is_classified<CharT>(loc, std::ctype_base::upper);
}

template <typename CharT = char>
inline auto is_xdigit(const std::locale& loc = std::locale()) 
    -> is_classified<CharT> {
    return make_is_classified<CharT>(loc, std::ctype_base::xdigit);
}

// 识别字符序列中的任意一个
template <typename CharT>
class is_any_of {
    const std::vector<CharT> chars;
public:
    explicit is_any_of(const std::vector<CharT>& ch) : chars(ch) {}
    
    bool operator()(CharT c) const {
        for (auto x : chars) {
            if (c == x) return true;
        }
        return false;
    }
};

template <typename CharT, typename RangeT>
inline auto make_is_any_of(const RangeT& range) -> is_any_of<CharT> {
    std::vector<CharT> chars;
    for (auto ch : range) {
        chars.push_back(ch);
    }
    return is_any_of<CharT>(chars);
}

// 识别 min..max 范围内的字符
template <typename CharT>
class is_from_range {
    const CharT min_c;
    const CharT max_c;
public:
    is_from_range(CharT minv, CharT maxv) : min_c(minv), max_c(maxv) {}
    
    bool operator()(CharT c) const {
        return c >= min_c && c <= max_c;
    }
};

// 不区分大小写的比较谓词
template <typename CharT>
class is_iequal {
    const std::locale& loc;
public:
    explicit is_iequal(const std::locale& loc_ = std::locale()) : loc(loc_) {}
    
    bool operator()(CharT a, CharT b) const {
        auto& ct = std::use_facet<std::ctype<CharT>>(loc);
        return ct.tolower(a) == ct.tolower(b);
    }
};

// 小于比较
template <typename CharT>
class is_less {
public:
    bool operator()(CharT a, CharT b) const { return a < b; }
};

template <typename CharT>
class is_not_greater {
public:
    bool operator()(CharT a, CharT b) const { return a <= b; }
};

} // namespace string_algo

#endif // STRING_ALGO_CLASSIFICATION_HPP
