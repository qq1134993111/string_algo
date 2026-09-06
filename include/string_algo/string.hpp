#ifndef STRING_ALGO_HPP
#define STRING_ALGO_HPP

// ============================================================================
// 纯标准 C++ 字符串算法
//
// 从 Boost.Algorithm 字符串算法中提取出的自包含实现，
// 仅依赖 C++ 标准库，无任何 Boost 依赖。
//
// 功能：
//   - 大小写转换：to_upper, to_lower
//   - 修剪：trim, trim_left, trim_right, trim_all
//   - 谓词：starts_with, ends_with, contains, equals
//   - 查找算法：find_first, find_last, find_nth, find_head, find_tail
//   - 替换/删除：replace_first, replace_all, erase_first, erase_all
//   - 分割：split, find_all, split_regex, find_all_regex
//   - 连接：join, join_if
//   - 分类：is_space, is_alpha, is_digit 等
//   - 查找器：first_finder, last_finder, nth_finder, head_finder, tail_finder, regex_finder
//   - 格式化器：const_formatter, identity_formatter, empty_formatter
//   - 迭代器：find_iterator, split_iterator
//   - 正则：find_regex, replace_regex, erase_regex
//   - 格式化：find_format, find_format_copy
//
// 用法：
//   #include <string_algo/string.hpp>
//   using namespace string_algo;
//
//   std::string s = " hello world! ";
//   trim(s);          // s == "hello world!"
//   to_upper(s);      // s == "HELLO WORLD!"
//   replace_all(s, "WORLD", "STANDARD C++"); // s == "HELLO STANDARD C++!"
//   // 正则示例：
//   std::regex re("[0-9]+");
//   erase_regex(s, re);  // 移除所有数字
// ============================================================================

#include "string/config.hpp"
#include "string/constants.hpp"
#include "string/case_conv.hpp"
#include "string/classification.hpp"
#include "string/trim.hpp"
#include "string/predicate.hpp"
#include "string/find_iterator.hpp"
#include "string/finder.hpp"
#include "string/find.hpp"
#include "string/replace.hpp"
#include "string/split.hpp"
#include "string/join.hpp"
#include "string/find_format.hpp"

// 正则支持（使用 std::regex）
#include <regex>
#include "string/find_regex.hpp"
#include "string/replace_regex.hpp"
#include "string/split_regex.hpp"

#endif // STRING_ALGO_HPP
