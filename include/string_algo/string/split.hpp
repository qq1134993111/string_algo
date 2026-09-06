#ifndef STRING_ALGO_SPLIT_HPP
#define STRING_ALGO_SPLIT_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
#include "config.hpp"
#include "constants.hpp"
#include "finder.hpp"
#include "find_iterator.hpp"

namespace string_algo {

namespace detail {

template <typename RangeT, typename PredT>
void split_impl(std::vector<RangeT>& result, RangeT& range, 
                PredT pred, token_compress_mode_type compress) {
    result.clear();
    
    auto first = range.begin();
    auto last = range.end();
    auto tok_start = first;
    bool in_token = false;
    bool last_was_delimiter = false;
    
    while (first != last) {
        if (!pred(*first)) {
            if (!in_token) {
                tok_start = first;
                in_token = true;
            }
            last_was_delimiter = false;
        } else {
            if (in_token) {
                result.emplace_back(tok_start, first);
                in_token = false;
            } else if (compress == token_compress_off) {
                // token_compress_off 模式下，连续分隔符产生空 token
                result.emplace_back(first, first);
            }
            last_was_delimiter = true;
        }
        ++first;
    }
    
    if (in_token) {
        result.emplace_back(tok_start, last);
    } else if (last_was_delimiter && compress == token_compress_off) {
        // token_compress_off 模式下，尾部分隔符产生空 token
        result.emplace_back(last, last);
    }
}

} // namespace detail

// ---- 分割算法 ----
template <typename RangeT, typename PredT>
void split(std::vector<RangeT>& result, RangeT& range, 
           PredT pred, token_compress_mode_type compress = token_compress_off) {
    detail::split_impl(result, range, pred, compress);
}

// ---- find_all（查找所有匹配）----
template <typename RangeT>
void find_all(std::vector<std::pair<
    typename RangeT::const_iterator, 
    typename RangeT::const_iterator>>& result, 
    RangeT& range, const std::string& sub) {
    result.clear();
    
    auto first = range.begin();
    auto last = range.end();
    auto sub_first = sub.begin();
    auto sub_last = sub.end();
    
    while (first != last) {
        auto it = first;
        auto sit = sub_first;
        bool found = true;
        while (it != last && sit != sub_last) {
            if (*it != *sit) {
                found = false;
                break;
            }
            ++it;
            ++sit;
        }
        if (found && sit == sub_last) {
            result.emplace_back(first, it);
            first = it;
        } else {
            ++first;
        }
    }
}

// 不区分大小写的 find_all
template <typename RangeT>
void ifind_all(std::vector<std::pair<
    typename RangeT::const_iterator, 
    typename RangeT::const_iterator>>& result, 
    RangeT& range, const std::string& sub) {
    result.clear();
    
    auto first = range.begin();
    auto last = range.end();
    auto sub_first = sub.begin();
    auto sub_last = sub.end();
    
    while (first != last) {
        auto it = first;
        auto sit = sub_first;
        bool found = true;
        while (it != last && sit != sub_last) {
            auto a = std::tolower(static_cast<unsigned char>(*it));
            auto b = std::tolower(static_cast<unsigned char>(*sit));
            if (a != b) {
                found = false;
                break;
            }
            ++it;
            ++sit;
        }
        if (found && sit == sub_last) {
            result.emplace_back(first, it);
            first = it;
        } else {
            ++first;
        }
    }
}

// 注意：find_format 已移至 find_format.hpp，此处不再定义

} // namespace string_algo

#endif // STRING_ALGO_SPLIT_HPP
