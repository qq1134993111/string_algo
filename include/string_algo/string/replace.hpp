#ifndef STRING_ALGO_REPLACE_HPP
#define STRING_ALGO_REPLACE_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
#include "config.hpp"
#include "finder.hpp"

namespace string_algo {

namespace detail {

template <typename Range1T, typename Range2T, typename Range3T>
std::pair<typename Range1T::iterator, typename Range1T::iterator>
replace_first_impl(Range1T& r1, const Range2T& r2, const Range3T& r3) {
    auto first = r1.begin();
    auto last = r1.end();
    auto sub = r2.begin();
    auto sub_end = r2.end();
    
    while (first != last) {
        auto it = first;
        auto sit = sub;
        bool found = true;
        while (it != last && sit != sub_end) {
            if (*it != *sit) {
                found = false;
                break;
            }
            ++it;
            ++sit;
        }
        if (found && sit == sub_end) {
            // 替换：先删除匹配段，再插入替换文本
            r1.erase(first, it);
            auto new_begin = r1.insert(first, r3.begin(), r3.end());
            auto new_end = new_begin;
            std::advance(new_end, std::distance(r3.begin(), r3.end()));
            return {new_begin, new_end};
        }
        ++first;
    }
    return {r1.end(), r1.end()};
}

} // namespace detail

// ---- 替换/删除算法 ----

template <typename Range1T, typename Range2T, typename Range3T>
std::pair<typename Range1T::iterator, typename Range1T::iterator>
replace_first(Range1T& r1, const Range2T& r2, const Range3T& r3) {
    return detail::replace_first_impl(r1, r2, r3);
}

template <typename Range1T, typename Range2T, typename Range3T>
Range1T replace_first_copy(const Range1T& r1, const Range2T& r2, const Range3T& r3) {
    Range1T result = r1;
    replace_first(result, r2, r3);
    return result;
}

template <typename Range1T, typename Range2T>
std::pair<typename Range1T::iterator, typename Range1T::iterator>
erase_first(Range1T& r1, const Range2T& r2) {
    auto first = r1.begin();
    auto last = r1.end();
    auto sub = r2.begin();
    auto sub_end = r2.end();
    
    while (first != last) {
        auto it = first;
        auto sit = sub;
        bool found = true;
        while (it != last && sit != sub_end) {
            if (*it != *sit) {
                found = false;
                break;
            }
            ++it;
            ++sit;
        }
        if (found && sit == sub_end) {
            r1.erase(first, it);
            return {first, first};
        }
        ++first;
    }
    return {r1.end(), r1.end()};
}

template <typename Range1T, typename Range2T>
Range1T erase_first_copy(const Range1T& r1, const Range2T& r2) {
    Range1T result = r1;
    erase_first(result, r2);
    return result;
}

// 不区分大小写的版本
template <typename Range1T, typename Range2T, typename Range3T>
std::pair<typename Range1T::iterator, typename Range1T::iterator>
ireplace_first(Range1T& r1, const Range2T& r2, const Range3T& r3) {
    auto first = r1.begin();
    auto last = r1.end();
    auto sub = r2.begin();
    auto sub_end = r2.end();
    
    while (first != last) {
        auto it = first;
        auto sit = sub;
        bool found = true;
        while (it != last && sit != sub_end) {
            auto a = std::tolower(static_cast<unsigned char>(*it));
            auto b = std::tolower(static_cast<unsigned char>(*sit));
            if (a != b) {
                found = false;
                break;
            }
            ++it;
            ++sit;
        }
        if (found && sit == sub_end) {
            // 替换：先删除匹配段，再插入替换文本
            r1.erase(first, it);
            auto new_begin = r1.insert(first, r3.begin(), r3.end());
            auto new_end = new_begin;
            std::advance(new_end, std::distance(r3.begin(), r3.end()));
            return {new_begin, new_end};
        }
        ++first;
    }
    return {r1.end(), r1.end()};
}

template <typename Range1T, typename Range2T, typename Range3T>
Range1T ireplace_first_copy(const Range1T& r1, const Range2T& r2, const Range3T& r3) {
    Range1T result = r1;
    ireplace_first(result, r2, r3);
    return result;
}

template <typename Range1T, typename Range2T>
std::pair<typename Range1T::iterator, typename Range1T::iterator>
ierase_first(Range1T& r1, const Range2T& r2) {
    auto first = r1.begin();
    auto last = r1.end();
    auto sub = r2.begin();
    auto sub_end = r2.end();
    
    while (first != last) {
        auto it = first;
        auto sit = sub;
        bool found = true;
        while (it != last && sit != sub_end) {
            auto a = std::tolower(static_cast<unsigned char>(*it));
            auto b = std::tolower(static_cast<unsigned char>(*sit));
            if (a != b) {
                found = false;
                break;
            }
            ++it;
            ++sit;
        }
        if (found && sit == sub_end) {
            r1.erase(first, it);
            return {first, first};
        }
        ++first;
    }
    return {r1.end(), r1.end()};
}

template <typename Range1T, typename Range2T>
Range1T ierase_first_copy(const Range1T& r1, const Range2T& r2) {
    Range1T result = r1;
    ierase_first(result, r2);
    return result;
}

// ---- replace_all / erase_all（全部替换/删除）----

template <typename Range1T, typename Range2T, typename Range3T>
void replace_all(Range1T& r1, const Range2T& r2, const Range3T& r3) {
    auto first = r1.begin();
    auto last = r1.end();
    
    while (first != last) {
        auto it = first;
        auto sit = r2.begin();
        auto sub_end = r2.end();
        bool found = true;
        while (it != last && sit != sub_end) {
            if (*it != *sit) {
                found = false;
                break;
            }
            ++it;
            ++sit;
        }
        if (found && sit == sub_end) {
            // 替换：先删除匹配段，再插入替换文本
            r1.erase(first, it);
            auto new_begin = r1.insert(first, r3.begin(), r3.end());
            // 跳过已插入的文本
            first = new_begin;
            std::advance(first, std::distance(r3.begin(), r3.end()));
            last = r1.end();
        } else {
            ++first;
        }
    }
}

template <typename Range1T, typename Range2T, typename Range3T>
Range1T replace_all_copy(const Range1T& r1, const Range2T& r2, const Range3T& r3) {
    Range1T result = r1;
    replace_all(result, r2, r3);
    return result;
}

template <typename Range1T, typename Range2T>
void erase_all(Range1T& r1, const Range2T& r2) {
    auto first = r1.begin();
    auto last = r1.end();
    
    while (first != last) {
        auto it = first;
        auto sit = r2.begin();
        auto sub_end = r2.end();
        bool found = true;
        while (it != last && sit != sub_end) {
            if (*it != *sit) {
                found = false;
                break;
            }
            ++it;
            ++sit;
        }
        if (found && sit == sub_end) {
            r1.erase(first, it);
            last = r1.end();
            // 不推进 first——再次检查同一位置
        } else {
            ++first;
        }
    }
}

template <typename Range1T, typename Range2T>
Range1T erase_all_copy(const Range1T& r1, const Range2T& r2) {
    Range1T result = r1;
    erase_all(result, r2);
    return result;
}

// 不区分大小写的 replace_all
template <typename Range1T, typename Range2T, typename Range3T>
void ireplace_all(Range1T& r1, const Range2T& r2, const Range3T& r3) {
    auto first = r1.begin();
    auto last = r1.end();
    
    while (first != last) {
        auto it = first;
        auto sit = r2.begin();
        auto sub_end = r2.end();
        bool found = true;
        while (it != last && sit != sub_end) {
            auto a = std::tolower(static_cast<unsigned char>(*it));
            auto b = std::tolower(static_cast<unsigned char>(*sit));
            if (a != b) {
                found = false;
                break;
            }
            ++it;
            ++sit;
        }
        if (found && sit == sub_end) {
            // 先删除匹配段，再插入替换文本（避免迭代器失效）
            r1.erase(first, it);
            auto new_begin = r1.insert(first, r3.begin(), r3.end());
            // 跳过已插入的文本
            first = new_begin;
            std::advance(first, std::distance(r3.begin(), r3.end()));
            last = r1.end();
        } else {
            ++first;
        }
    }
}

template <typename Range1T, typename Range2T, typename Range3T>
Range1T ireplace_all_copy(const Range1T& r1, const Range2T& r2, const Range3T& r3) {
    Range1T result = r1;
    ireplace_all(result, r2, r3);
    return result;
}

// 不区分大小写的 erase_all
template <typename Range1T, typename Range2T>
void ierase_all(Range1T& r1, const Range2T& r2) {
    auto first = r1.begin();
    auto last = r1.end();
    
    while (first != last) {
        auto it = first;
        auto sit = r2.begin();
        auto sub_end = r2.end();
        bool found = true;
        while (it != last && sit != sub_end) {
            auto a = std::tolower(static_cast<unsigned char>(*it));
            auto b = std::tolower(static_cast<unsigned char>(*sit));
            if (a != b) {
                found = false;
                break;
            }
            ++it;
            ++sit;
        }
        if (found && sit == sub_end) {
            r1.erase(first, it);
            last = r1.end();
        } else {
            ++first;
        }
    }
}

template <typename Range1T, typename Range2T>
Range1T ierase_all_copy(const Range1T& r1, const Range2T& r2) {
    Range1T result = r1;
    ierase_all(result, r2);
    return result;
}

// ---- 替换/删除头部 ----

template <typename Range1T, typename Range3T>
void replace_head(Range1T& r1, std::ptrdiff_t idx, const Range3T& r3) {
    auto first = r1.begin();
    auto last = r1.end();
    auto len = std::distance(first, last);
    
    if (idx < 0) {
        // 负索引：保留从 idx 到末尾的子串（保留 -idx 个字符）
        if (static_cast<std::size_t>(-idx) > static_cast<std::size_t>(len)) return;
        auto cut = first;
        std::advance(cut, len + idx);
        r1.erase(cut, last);
        r1.insert(cut, r3.begin(), r3.end());
    } else if (static_cast<std::size_t>(idx) >= static_cast<std::size_t>(len)) {
        return;
    } else {
        // 正索引：替换从 idx 到末尾的部分
        std::advance(first, idx);
        r1.erase(first, last);
        r1.insert(first, r3.begin(), r3.end());
    }
}

template <typename Range1T, typename Range3T>
Range1T replace_head_copy(const Range1T& r1, std::ptrdiff_t idx, const Range3T& r3) {
    Range1T result = r1;
    replace_head(result, idx, r3);
    return result;
}

template <typename Range1T>
void erase_head(Range1T& r1, std::ptrdiff_t idx) {
    auto first = r1.begin();
    auto last = r1.end();
    
    if (idx < 0) {
        if (static_cast<std::size_t>(-idx) >= std::distance(first, last)) return;
        std::advance(first, static_cast<std::ptrdiff_t>(std::distance(first, last) + idx));
    } else if (static_cast<std::size_t>(idx) >= std::distance(first, last)) {
        return;
    } else {
        std::advance(first, idx);
    }
    
    r1.erase(first, last);
}

template <typename Range1T>
Range1T erase_head_copy(const Range1T& r1, std::ptrdiff_t idx) {
    Range1T result = r1;
    erase_head(result, idx);
    return result;
}

// ---- 替换/删除尾部 ----

template <typename Range1T, typename Range3T>
void replace_tail(Range1T& r1, std::ptrdiff_t idx, const Range3T& r3) {
    auto first = r1.begin();
    auto last = r1.end();
    
    if (idx < 0) {
        if (static_cast<std::size_t>(-idx) >= std::distance(first, last)) return;
        std::advance(first, static_cast<std::ptrdiff_t>(std::distance(first, last) + idx));
    } else if (static_cast<std::size_t>(idx) >= std::distance(first, last)) {
        return;
    } else {
        std::advance(first, idx);
    }
    
    r1.erase(first, last);
    r1.insert(first, r3.begin(), r3.end());
}

template <typename Range1T, typename Range3T>
Range1T replace_tail_copy(const Range1T& r1, std::ptrdiff_t idx, const Range3T& r3) {
    Range1T result = r1;
    replace_tail(result, idx, r3);
    return result;
}

template <typename Range1T>
void erase_tail(Range1T& r1, std::ptrdiff_t idx) {
    auto first = r1.begin();
    auto last = r1.end();
    
    if (idx < 0) {
        if (static_cast<std::size_t>(-idx) >= std::distance(first, last)) return;
        std::advance(first, static_cast<std::ptrdiff_t>(std::distance(first, last) + idx));
    } else if (static_cast<std::size_t>(idx) >= std::distance(first, last)) {
        return;
    } else {
        std::advance(first, idx);
    }
    
    r1.erase(first, last);
}

template <typename Range1T>
Range1T erase_tail_copy(const Range1T& r1, std::ptrdiff_t idx) {
    Range1T result = r1;
    erase_tail(result, idx);
    return result;
}

} // namespace string_algo

#endif // STRING_ALGO_REPLACE_HPP
