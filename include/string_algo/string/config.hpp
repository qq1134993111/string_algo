#ifndef STRING_ALGO_CONFIG_HPP
#define STRING_ALGO_CONFIG_HPP

// 纯标准 C++ 字符串算法 —— 不依赖任何 Boost 库

// 使用 std::iterator_traits 替代 Boost.Range
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>

// 获取序列迭代器类型的辅助工具
namespace string_algo {
namespace detail {

// 检测 T 是否是 range（具有 begin() 和 end()）
template <typename T, typename = void>
struct is_range : std::false_type {};

template <typename T>
struct is_range<T, std::void_t<decltype(std::declval<T&>().begin()),
                              decltype(std::declval<T&>().end())>> : std::true_type {};

// C++17 range 迭代器类型辅助工具
template <typename T>
using range_iterator_t = decltype(std::declval<T&>().begin());

template <typename T>
using range_const_iterator_t = decltype(std::declval<const T&>().begin());

template <typename T>
using range_value_t = typename std::iterator_traits<range_iterator_t<T>>::value_type;

} // namespace detail
} // namespace string_algo

#endif // STRING_ALGO_CONFIG_HPP
