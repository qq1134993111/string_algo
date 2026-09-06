#ifndef STRING_ALGO_PREDICATE_HPP
#define STRING_ALGO_PREDICATE_HPP

#include <algorithm>
#include <iterator>
#include "config.hpp"
#include "classification.hpp"

namespace string_algo {

// ---- 字符串谓词 ----

template <typename Range1T, typename Range2T>
bool starts_with(const Range1T& r1, const Range2T& r2) {
    auto it1 = r1.begin();
    auto it2 = r2.begin();
    while (it2 != r2.end()) {
        if (it1 == r1.end() || *it1 != *it2) return false;
        ++it1;
        ++it2;
    }
    return true;
}

template <typename Range1T, typename Range2T>
bool istarts_with(const Range1T& r1, const Range2T& r2) {
    auto it1 = r1.begin();
    auto it2 = r2.begin();
    while (it2 != r2.end()) {
        if (it1 == r1.end()) return false;
        auto a = std::tolower(static_cast<unsigned char>(*it1));
        auto b = std::tolower(static_cast<unsigned char>(*it2));
        if (a != b) return false;
        ++it1;
        ++it2;
    }
    return true;
}

template <typename Range1T, typename Range2T>
bool ends_with(const Range1T& r1, const Range2T& r2) {
    auto it1 = r1.end();
    auto it2 = r2.end();
    while (it2 != r2.begin()) {
        if (it1 == r1.begin()) return false;
        --it1;
        --it2;
        if (*it1 != *it2) return false;
    }
    return true;
}

template <typename Range1T, typename Range2T>
bool iends_with(const Range1T& r1, const Range2T& r2) {
    auto it1 = r1.end();
    auto it2 = r2.end();
    while (it2 != r2.begin()) {
        if (it1 == r1.begin()) return false;
        --it1;
        --it2;
        auto a = std::tolower(static_cast<unsigned char>(*it1));
        auto b = std::tolower(static_cast<unsigned char>(*it2));
        if (a != b) return false;
    }
    return true;
}

template <typename Range1T, typename Range2T>
bool contains(const Range1T& r1, const Range2T& r2) {
    if (r2.empty()) return true;
    if (r1.empty()) return false;
    
    auto r2_first = r2.begin();
    auto r2_last = r2.end();
    
    auto r1_it = r1.begin();
    auto r1_last = r1.end();
    
    while (r1_it != r1_last) {
        if (*r1_it == *r2_first) {
            auto it = r1_it;
            auto it2 = r2_first;
            bool found = true;
            while (it != r1_last && it2 != r2_last) {
                if (*it != *it2) {
                    found = false;
                    break;
                }
                ++it;
                ++it2;
            }
            if (found && it2 == r2_last) return true;
        }
        ++r1_it;
    }
    return false;
}

template <typename Range1T, typename Range2T>
bool icontains(const Range1T& r1, const Range2T& r2) {
    if (r2.empty()) return true;
    if (r1.empty()) return false;
    
    auto r2_first = r2.begin();
    auto r2_last = r2.end();
    
    auto r1_it = r1.begin();
    auto r1_last = r1.end();
    
    while (r1_it != r1_last) {
        auto a = std::tolower(static_cast<unsigned char>(*r1_it));
        auto b = std::tolower(static_cast<unsigned char>(*r2_first));
        if (a == b) {
            auto it = r1_it;
            auto it2 = r2_first;
            bool found = true;
            while (it != r1_last && it2 != r2_last) {
                auto a2 = std::tolower(static_cast<unsigned char>(*it));
                auto b2 = std::tolower(static_cast<unsigned char>(*it2));
                if (a2 != b2) {
                    found = false;
                    break;
                }
                ++it;
                ++it2;
            }
            if (found && it2 == r2_last) return true;
        }
        ++r1_it;
    }
    return false;
}

template <typename Range1T, typename Range2T>
bool equals(const Range1T& r1, const Range2T& r2) {
    auto it1 = r1.begin();
    auto it2 = r2.begin();
    while (it1 != r1.end() && it2 != r2.end()) {
        if (*it1 != *it2) return false;
        ++it1;
        ++it2;
    }
    return it1 == r1.end() && it2 == r2.end();
}

template <typename Range1T, typename Range2T>
bool iequals(const Range1T& r1, const Range2T& r2) {
    auto it1 = r1.begin();
    auto it2 = r2.begin();
    while (it1 != r1.end() && it2 != r2.end()) {
        auto a = std::tolower(static_cast<unsigned char>(*it1));
        auto b = std::tolower(static_cast<unsigned char>(*it2));
        if (a != b) return false;
        ++it1;
        ++it2;
    }
    return it1 == r1.end() && it2 == r2.end();
}

template <typename Range1T, typename Range2T>
bool lexicographical_compare(const Range1T& r1, const Range2T& r2) {
    return std::lexicographical_compare(r1.begin(), r1.end(), r2.begin(), r2.end());
}

template <typename Range1T, typename Range2T>
bool ilexicographical_compare(const Range1T& r1, const Range2T& r2) {
    auto it1 = r1.begin();
    auto it2 = r2.begin();
    while (it1 != r1.end() && it2 != r2.end()) {
        auto a = std::tolower(static_cast<unsigned char>(*it1));
        auto b = std::tolower(static_cast<unsigned char>(*it2));
        if (a != b) return a < b;
        ++it1;
        ++it2;
    }
    return it1 == r1.end() && it2 != r2.end();
}

template <typename RangeT, typename PredT>
bool all(const RangeT& range, PredT pred) {
    return std::all_of(range.begin(), range.end(), pred);
}

} // namespace string_algo

#endif // STRING_ALGO_PREDICATE_HPP
