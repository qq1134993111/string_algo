#ifndef STRING_ALGO_FIND_ITERATOR_HPP
#define STRING_ALGO_FIND_ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include "config.hpp"

namespace string_algo {

namespace detail {

// 辅助函数：检查范围是否为空
template <typename RangeT>
bool is_empty(const RangeT& r) {
    return r.begin() == r.end();
}

} // namespace detail

// find_iterator 遍历匹配的子串
template <typename FinderT, typename IterT, typename RangeT>
class find_iterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = IterT;
    using difference_type = std::ptrdiff_t;
    using pointer = IterT*;
    using reference = IterT&;

    std::pair<IterT, IterT> seq_range;
    std::pair<IterT, IterT> match_range;
    std::shared_ptr<FinderT> finder;
    RangeT range;
    bool eof;

    // 默认构造：创建哨兵迭代器（eof = true）
    // 使用 shared_ptr 替代 std::optional，避免要求 FinderT 可默认构造（C++11 兼容）
    find_iterator()
        : seq_range(), match_range(), finder(nullptr), range(), eof(true) {}

    explicit find_iterator(const FinderT& f, IterT first, IterT last, const RangeT& r = RangeT())
        : seq_range(first, last), match_range(), finder(new FinderT(f)), range(r), eof(false) {
        advance();
    }

    void advance() {
        if (eof || !finder) return;
        match_range = finder->operator()(seq_range);
        // 查找器在未找到匹配时返回 {seq_range.second, seq_range.second}
        // 因此检查 match_range.first 是否为序列末尾来判断是否还有匹配
        if (match_range.first == seq_range.second) {
            eof = true;
        } else {
            seq_range.first = match_range.second;
        }
    }

    // 注意：原代码的哨兵 operator= 已移除，因为与 shared_ptr 版本冲突

    bool is_empty() const { return match_range.first == match_range.second; }
    const std::pair<IterT, IterT>& get() const { return match_range; }
    const RangeT& get_range() const { return range; }

    find_iterator& operator++() {
        advance();
        return *this;
    }

    void operator++(int) { advance(); }

    bool operator==(const find_iterator& other) const {
        if (eof && other.eof) return true;
        if (eof != other.eof) return false;
        return match_range == other.match_range;
    }

    bool operator!=(const find_iterator& other) const { return !(*this == other); }

    IterT begin() const { return match_range.first; }
    IterT end() const { return match_range.second; }

    IterT operator*() const { return match_range.first; }
};

// 便捷构造函数
template <typename FinderT, typename RangeT>
inline find_iterator<FinderT, 
    decltype(std::declval<RangeT&>().begin()), 
    RangeT>
make_find_iterator(const FinderT& finder, RangeT& range) {
    return find_iterator<FinderT, 
        decltype(std::declval<RangeT&>().begin()), 
        RangeT>(finder, range.begin(), range.end(), range);
}

// split_iterator 遍历匹配子串之间的间隔
template <typename FinderT, typename IterT, typename RangeT>
class split_iterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = IterT;
    using difference_type = std::ptrdiff_t;
    using pointer = IterT*;
    using reference = IterT&;

private:
    std::pair<IterT, IterT> seq_range;
    std::pair<IterT, IterT> match_range;
    std::shared_ptr<FinderT> finder;
    RangeT range;
    bool eof;

    // 默认构造：创建哨兵迭代器
    split_iterator()
        : seq_range(), match_range(), finder(nullptr), range(), eof(true) {}

    void advance() {
        if (eof || !finder) return;
        std::pair<IterT, IterT> prev = match_range;
        match_range = finder->operator()(seq_range);
        if (match_range.first == seq_range.second) {
            eof = true;
        } else {
            seq_range.first = match_range.second;
        }
        if (prev.second != match_range.first) {
            seq_range.first = match_range.first;
        }
    }

public:
    explicit split_iterator(const FinderT& f, IterT first, IterT last, const RangeT& r = RangeT())
        : seq_range(first, last), match_range(), finder(new FinderT(f)), range(r), eof(false) {
        match_range = finder->operator()(seq_range);
        if (match_range.first == seq_range.second) {
            match_range.first = seq_range.first;
            eof = true;
        }
    }

    bool is_empty() const { return match_range.first == match_range.second; }
    const std::pair<IterT, IterT>& get() const { return match_range; }

    split_iterator& operator++() {
        advance();
        return *this;
    }

    void operator++(int) { advance(); }

    bool operator==(const split_iterator& other) const {
        if (eof && other.eof) return true;
        if (eof != other.eof) return false;
        return match_range == other.match_range;
    }

    bool operator!=(const split_iterator& other) const { return !(*this == other); }

    IterT begin() const { return match_range.first; }
    IterT end() const { return match_range.second; }

    IterT operator*() const { return match_range.first; }
};

// 便捷构造函数
template <typename FinderT, typename RangeT>
inline auto make_split_iterator(const FinderT& finder, RangeT& range) 
    -> split_iterator<FinderT, decltype(range.begin()), RangeT> {
    using iter_t = decltype(range.begin());
    return split_iterator<FinderT, iter_t, RangeT>(finder, range.begin(), range.end(), range);
}

} // namespace string_algo

#endif // STRING_ALGO_FIND_ITERATOR_HPP
