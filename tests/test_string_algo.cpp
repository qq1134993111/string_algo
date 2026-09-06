// ============================================================================
// string_algo 单元测试
//
// 参考 Boost.Algorithm string/test/ 下的官方测试用例风格：
//   conv_test.cpp, trim_test.cpp, predicate_test.cpp,
//   find_test.cpp, split_test.cpp, replace_test.cpp
//
// 仅依赖 C++ 标准库，使用 assert 进行断言。
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstring>
#include <functional>
#include "../include/string_algo/string.hpp"

using namespace string_algo;

// ---------------------------------------------------------------------------
// 测试基础设施
// ---------------------------------------------------------------------------
int g_pass_count = 0;
int g_fail_count = 0;

#define TEST_ALGO(expr) do { \
    if (expr) { ++g_pass_count; } \
    else { ++g_fail_count; \
           std::cerr << "  FAIL: " << #expr << " @ line " << __LINE__ << std::endl; } \
} while (0)

#define TEST_SECTION(name) std::cout << "Testing " << name << "...\n"

// ===========================================================================
// 1. 大小写转换 —— 对应 Boost conv_test.cpp
// ===========================================================================
void test_case_conversion() {
    TEST_SECTION("case conversion");

    // to_upper / to_upper_copy
    {
        std::string s = "hello world";
        to_upper(s);
        TEST_ALGO(s == "HELLO WORLD");
    }
    {
        std::string s = "hello world";
        std::string r = to_upper_copy(s);
        TEST_ALGO(r == "HELLO WORLD");
        TEST_ALGO(s == "hello world"); // 原串不变
    }

    // to_lower / to_lower_copy
    {
        std::string s = "HELLO WORLD";
        to_lower(s);
        TEST_ALGO(s == "hello world");
    }
    {
        std::string s = "HELLO WORLD";
        std::string r = to_lower_copy(s);
        TEST_ALGO(r == "hello world");
        TEST_ALGO(s == "HELLO WORLD"); // 原串不变
    }

    // 混合大小写
    {
        std::string s = "Hello World!";
        to_upper(s);
        TEST_ALGO(s == "HELLO WORLD!");
    }
    {
        std::string s = "Hello World!";
        to_lower(s);
        TEST_ALGO(s == "hello world!");
    }

    // 纯数字与符号
    {
        std::string s = "abc123def";
        to_upper(s);
        TEST_ALGO(s == "ABC123DEF");
    }

    std::cout << "  Case conversion tests passed.\n";
}

// ===========================================================================
// 2. 修剪 —— 对应 Boost trim_test.cpp
// ===========================================================================
void test_trimming() {
    TEST_SECTION("trimming");

    // trim / trim_left / trim_right
    {
        std::string s = "  hello world  ";
        trim(s);
        TEST_ALGO(s == "hello world");
    }
    {
        std::string s = "   hello";
        trim_left(s);
        TEST_ALGO(s == "hello");
    }
    {
        std::string s = "hello   ";
        trim_right(s);
        TEST_ALGO(s == "hello");
    }

    // 纯空格
    {
        std::string s = "   ";
        trim(s);
        TEST_ALGO(s.empty());
    }

    // 无空格
    {
        std::string s = "hello";
        trim(s);
        TEST_ALGO(s == "hello");
    }

    // trim_copy 系列
    {
        std::string s = "  hello  ";
        std::string r = trim_copy(s);
        TEST_ALGO(r == "hello");
        TEST_ALGO(s == "  hello  "); // 原串不变
    }
    {
        std::string s = "   hello";
        std::string r = trim_left_copy(s);
        TEST_ALGO(r == "hello");
        TEST_ALGO(s == "   hello");
    }
    {
        std::string s = "hello   ";
        std::string r = trim_right_copy(s);
        TEST_ALGO(r == "hello");
        TEST_ALGO(s == "hello   ");
    }

    // trim_if（自定义谓词）
    {
        std::string s = "  hello  ";
        trim_if(s, [](char c) { return c == ' '; });
        TEST_ALGO(s == "hello");
    }
    {
        std::string s = "---hello---";
        trim_if(s, [](char c) { return c == '-'; });
        TEST_ALGO(s == "hello");
    }

    // trim_left_if / trim_right_if
    {
        std::string s = "---hello---";
        trim_left_if(s, [](char c) { return c == '-'; });
        TEST_ALGO(s == "hello---");
    }
    {
        std::string s = "---hello---";
        trim_right_if(s, [](char c) { return c == '-'; });
        TEST_ALGO(s == "---hello");
    }

    // trim_all（压缩内部空白）
    {
        std::string s = "  hello   world  ";
        trim_all(s);
        TEST_ALGO(s == "hello world");
    }
    {
        std::string s = "  hello\t\tworld  ";
        trim_all(s);
        TEST_ALGO(s == "hello world");
    }
    {
        std::string s = "  hello   world\t\t  ";
        trim_all(s);
        TEST_ALGO(s == "hello world");
    }

    // trim_all_copy
    {
        std::string s = "  hello   world  ";
        std::string r = trim_all_copy(s);
        TEST_ALGO(r == "hello world");
        TEST_ALGO(s == "  hello   world  ");
    }

    // trim_all_if（自定义谓词）
    {
        std::string s = "  hello---world  ";
        trim_all_if(s, [](char c) { return c == '-' || c == ' '; });
        TEST_ALGO(s == "hello world");
    }

    // trim_all_copy_if
    {
        std::string s = "  hello---world  ";
        std::string r = trim_all_copy_if(s, [](char c) { return c == '-' || c == ' '; });
        TEST_ALGO(r == "hello world");
        TEST_ALGO(s == "  hello---world  ");
    }

    std::cout << "  Trimming tests passed.\n";
}

// ===========================================================================
// 3. 字符串谓词 —— 对应 Boost predicate_test.cpp
// ===========================================================================
void test_predicates() {
    TEST_SECTION("predicates");

    // starts_with / istarts_with
    {
        std::string s1 = "hello world";
        TEST_ALGO(starts_with(s1, std::string("hello")));
        TEST_ALGO(!starts_with(s1, std::string("world")));
        TEST_ALGO(!starts_with(s1, std::string("hello world!"))); // 更长则 false
        TEST_ALGO(starts_with(s1, std::string(""))); // 空子串
    }
    {
        std::string s1 = "Hello World";
        TEST_ALGO(istarts_with(s1, std::string("hello")));
        TEST_ALGO(istarts_with(s1, std::string("HELLO")));
        TEST_ALGO(!istarts_with(s1, std::string("world")));
    }

    // ends_with / iends_with
    {
        std::string s1 = "hello world";
        TEST_ALGO(ends_with(s1, std::string("world")));
        TEST_ALGO(!ends_with(s1, std::string("hello")));
        TEST_ALGO(ends_with(s1, std::string(""))); // 空子串
    }
    {
        std::string s1 = "Hello World";
        TEST_ALGO(iends_with(s1, std::string("world")));
        TEST_ALGO(iends_with(s1, std::string("WORLD")));
        TEST_ALGO(!iends_with(s1, std::string("hello")));
    }

    // contains / icontains
    {
        std::string s1 = "hello world";
        TEST_ALGO(contains(s1, std::string("world")));
        TEST_ALGO(contains(s1, std::string("lo wo")));
        TEST_ALGO(!contains(s1, std::string("xyz")));
        TEST_ALGO(contains(s1, std::string(""))); // 空子串
        TEST_ALGO(!contains(std::string(""), std::string("a"))); // 空主串
    }
    {
        std::string s1 = "Hello World";
        TEST_ALGO(icontains(s1, std::string("world")));
        TEST_ALGO(icontains(s1, std::string("WORLD")));
        TEST_ALGO(icontains(s1, std::string("lo wo")));
        TEST_ALGO(!icontains(s1, std::string("xyz")));
    }

    // equals / iequals
    {
        TEST_ALGO(equals(std::string("hello"), std::string("hello")));
        TEST_ALGO(!equals(std::string("hello"), std::string("world")));
        TEST_ALGO(!equals(std::string("hello"), std::string("Hello"))); // 区分大小写
        TEST_ALGO(equals(std::string("hello"), std::string("hello"))); // 自身
    }
    {
        TEST_ALGO(iequals(std::string("Hello"), std::string("hello")));
        TEST_ALGO(iequals(std::string("HELLO"), std::string("hello")));
        TEST_ALGO(!iequals(std::string("Hello"), std::string("World")));
    }

    // lexicographical_compare / ilexicographical_compare
    {
        TEST_ALGO(lexicographical_compare(std::string("apple"), std::string("banana")));
        TEST_ALGO(!lexicographical_compare(std::string("banana"), std::string("apple")));
        TEST_ALGO(!lexicographical_compare(std::string("apple"), std::string("apple"))); // 相等返回 false
    }
    {
        TEST_ALGO(ilexicographical_compare(std::string("Apple"), std::string("banana")));
        TEST_ALGO(!ilexicographical_compare(std::string("banana"), std::string("apple")));
    }

    // all
    {
        TEST_ALGO(all(std::string("12345"), [](char c) { return c >= '1' && c <= '5'; }));
        TEST_ALGO(!all(std::string("12345"), [](char c) { return c == '1'; }));
    }

    std::cout << "  Predicate tests passed.\n";
}

// ===========================================================================
// 4. 查找 —— 对应 Boost find_test.cpp
// ===========================================================================
void test_find() {
    TEST_SECTION("find");

    // find_first / ifind_first
    {
        std::string s = "hello world";
        TEST_ALGO(find_first(s, std::string("world")));
        TEST_ALGO(!find_first(s, std::string("xyz")));
        TEST_ALGO(find_first(s, std::string(""))); // 空串
    }
    {
        std::string s = "Hello World";
        TEST_ALGO(ifind_first(s, std::string("hello")));
        TEST_ALGO(ifind_first(s, std::string("WORLD")));
        TEST_ALGO(!ifind_first(s, std::string("xyz")));
    }

    std::cout << "  Find tests passed.\n";
}

// ===========================================================================
// 5. 替换 —— 对应 Boost replace_test.cpp
// ===========================================================================
void test_replace() {
    TEST_SECTION("replace");

    // replace_first
    {
        std::string s = "hello hello hello";
        replace_first(s, std::string("hello"), std::string("goodbye"));
        TEST_ALGO(s == "goodbye hello hello");
    }
    {
        std::string s = "hello hello hello";
        replace_first(s, std::string("xyz"), std::string("abc"));
        TEST_ALGO(s == "hello hello hello"); // 未找到，不变
    }

    // replace_first_copy
    {
        std::string s = "hello hello";
        std::string r = replace_first_copy(s, std::string("hello"), std::string("goodbye"));
        TEST_ALGO(r == "goodbye hello");
        TEST_ALGO(s == "hello hello"); // 原串不变
    }

    // replace_all
    {
        std::string s = "hello hello hello";
        replace_all(s, std::string("hello"), std::string("goodbye"));
        TEST_ALGO(s == "goodbye goodbye goodbye");
    }
    {
        std::string s = "hello hello hello";
        replace_all(s, std::string("xyz"), std::string("abc"));
        TEST_ALGO(s == "hello hello hello"); // 未找到
    }

    // replace_all_copy
    {
        std::string s = "hello hello";
        std::string r = replace_all_copy(s, std::string("hello"), std::string("goodbye"));
        TEST_ALGO(r == "goodbye goodbye");
        TEST_ALGO(s == "hello hello");
    }

    // 不区分大小写版本
    {
        std::string s = "Hello hello HELLO";
        ireplace_first(s, std::string("hello"), std::string("world"));
        TEST_ALGO(s == "world hello HELLO");
    }
    {
        std::string s = "Hello hello HELLO";
        ireplace_all(s, std::string("hello"), std::string("world"));
        TEST_ALGO(s == "world world world");
    }
    {
        std::string s = "Hello hello HELLO";
        std::string r = ireplace_first_copy(s, std::string("hello"), std::string("world"));
        TEST_ALGO(r == "world hello HELLO");
        TEST_ALGO(s == "Hello hello HELLO");
    }
    {
        std::string s = "Hello hello HELLO";
        std::string r = ireplace_all_copy(s, std::string("hello"), std::string("world"));
        TEST_ALGO(r == "world world world");
        TEST_ALGO(s == "Hello hello HELLO");
    }

    // erase_first / erase_all
    {
        std::string s = "hello world hello";
        erase_first(s, std::string("hello"));
        TEST_ALGO(s == " world hello");
    }
    {
        std::string s = "hello world hello";
        erase_all(s, std::string("hello"));
        TEST_ALGO(s == " world ");
    }
    {
        std::string s = "hello world hello";
        std::string r = erase_first_copy(s, std::string("hello"));
        TEST_ALGO(r == " world hello");
        TEST_ALGO(s == "hello world hello");
    }
    {
        std::string s = "hello world hello";
        std::string r = erase_all_copy(s, std::string("hello"));
        TEST_ALGO(r == " world ");
        TEST_ALGO(s == "hello world hello");
    }

    // 不区分大小写的 erase
    {
        std::string s = "Hello hello HELLO";
        ierase_first(s, std::string("hello"));
        TEST_ALGO(s == " hello HELLO");
    }
    {
        std::string s = "Hello hello HELLO";
        ierase_all(s, std::string("hello"));
        TEST_ALGO(s == "  ");
    }
    {
        std::string s = "Hello hello HELLO";
        std::string r = ierase_first_copy(s, std::string("hello"));
        TEST_ALGO(r == " hello HELLO");
        TEST_ALGO(s == "Hello hello HELLO");
    }
    {
        std::string s = "Hello hello HELLO";
        std::string r = ierase_all_copy(s, std::string("hello"));
        TEST_ALGO(r == "  ");
        TEST_ALGO(s == "Hello hello HELLO");
    }

    // replace_head / erase_head
    // 注意：idx=6 对应 "hello world" 中 'w' 的位置（0-based）
    // 保留前6个字符 "hello "，替换/删除剩余部分 "world"
    {
        std::string s = "hello world";
        replace_head(s, 6, std::string("STANDARD C++"));
        TEST_ALGO(s == "hello STANDARD C++");
    }
    {
        std::string s = "hello world";
        replace_head(s, 6, std::string("")); // 替换为空，保留 "hello "
        TEST_ALGO(s == "hello ");
    }
    {
        std::string s = "hello world";
        erase_head(s, 6);
        TEST_ALGO(s == "hello ");
    }
    {
        std::string s = "hello world";
        std::string r = replace_head_copy(s, 6, std::string("STANDARD C++"));
        TEST_ALGO(r == "hello STANDARD C++");
        TEST_ALGO(s == "hello world");
    }
    {
        std::string s = "hello world";
        std::string r = erase_head_copy(s, 6);
        TEST_ALGO(r == "hello ");
        TEST_ALGO(s == "hello world");
    }

    // replace_tail / erase_tail
    {
        std::string s = "hello world";
        replace_tail(s, 6, std::string("STANDARD C++"));
        TEST_ALGO(s == "hello STANDARD C++");
    }
    {
        std::string s = "hello world";
        erase_tail(s, 6);
        TEST_ALGO(s == "hello ");
    }
    {
        std::string s = "hello world";
        std::string r = replace_tail_copy(s, 6, std::string("STANDARD C++"));
        TEST_ALGO(r == "hello STANDARD C++");
        TEST_ALGO(s == "hello world");
    }
    {
        std::string s = "hello world";
        std::string r = erase_tail_copy(s, 6);
        TEST_ALGO(r == "hello ");
        TEST_ALGO(s == "hello world");
    }

    std::cout << "  Replace tests passed.\n";
}

// ===========================================================================
// 6. 分割 —— 对应 Boost split_test.cpp
// ===========================================================================
void test_split() {
    TEST_SECTION("split");

    // 基本分割
    {
        std::string s = "a,b,c,d";
        std::vector<std::string> result;
        split(result, s, [](char c) { return c == ','; }, token_compress_on);
        TEST_ALGO(result.size() == 4);
        TEST_ALGO(result[0] == "a");
        TEST_ALGO(result[1] == "b");
        TEST_ALGO(result[2] == "c");
        TEST_ALGO(result[3] == "d");
    }

    // 含空标记的分割
    {
        std::string s = "a,,b,c";
        std::vector<std::string> result;
        split(result, s, [](char c) { return c == ','; }, token_compress_off);
        TEST_ALGO(result.size() == 4);
        TEST_ALGO(result[1] == ""); // 空标记
    }

    // 压缩模式
    {
        std::string s = "a,,,b";
        std::vector<std::string> result;
        split(result, s, [](char c) { return c == ','; }, token_compress_on);
        TEST_ALGO(result.size() == 2);
        TEST_ALGO(result[0] == "a");
        TEST_ALGO(result[1] == "b");
    }

    // 尾部分隔符（token_compress_off 模式下尾部分隔符产生空 token）
    {
        std::string s = "a,b,c,";
        std::vector<std::string> result;
        split(result, s, [](char c) { return c == ','; }, token_compress_off);
        // 结果: ["a", "b", "c", ""] — 尾部分隔符产生空 token
        TEST_ALGO(result.size() == 4);
        TEST_ALGO(result[3] == "");
    }
    // 尾部分隔符（token_compress_on 模式下尾部分隔符不产生空 token）
    {
        std::string s = "a,b,c,";
        std::vector<std::string> result;
        split(result, s, [](char c) { return c == ','; }, token_compress_on);
        // 结果: ["a", "b", "c"] — 尾部分隔符被抑制
        TEST_ALGO(result.size() == 3);
        TEST_ALGO(result[0] == "a");
        TEST_ALGO(result[1] == "b");
        TEST_ALGO(result[2] == "c");
    }

    // 无分隔符
    {
        std::string s = "abcdef";
        std::vector<std::string> result;
        split(result, s, [](char c) { return c == ','; }, token_compress_off);
        TEST_ALGO(result.size() == 1);
        TEST_ALGO(result[0] == "abcdef");
    }

    std::cout << "  Split tests passed.\n";
}

// ===========================================================================
// 7. 连接 —— join
// ===========================================================================
void test_join() {
    TEST_SECTION("join");

    // 基本 join
    {
        std::vector<std::string> parts = {"hello", " ", "world"};
        std::string result = join(parts, std::string());
        TEST_ALGO(result == "hello world");
    }

    // 空容器
    {
        std::vector<std::string> parts;
        std::string result = join(parts, std::string());
        TEST_ALGO(result.empty());
    }

    // join_if（过滤后连接）
    {
        std::vector<std::string> parts = {"hello", "!", "world", "!", "!"};
        std::string result = join_if(parts, std::string(),
            [](const std::string& s) { return s != "!"; });
        TEST_ALGO(result == "helloworld");
    }

    std::cout << "  Join tests passed.\n";
}

// ===========================================================================
// 8. 字符分类 —— classification
// ===========================================================================
void test_classification() {
    TEST_SECTION("classification");

    // is_space
    {
        auto pred = is_space<char>();
        TEST_ALGO(pred(' '));
        TEST_ALGO(pred('\t'));
        TEST_ALGO(pred('\n'));
        TEST_ALGO(pred('\r'));
        TEST_ALGO(!pred('a'));
        TEST_ALGO(!pred('1'));
    }

    // is_alpha
    {
        auto pred = is_alpha<char>();
        TEST_ALGO(pred('a'));
        TEST_ALGO(pred('Z'));
        TEST_ALGO(!pred('1'));
        TEST_ALGO(!pred(' '));
    }

    // is_digit
    {
        auto pred = is_digit<char>();
        TEST_ALGO(pred('0'));
        TEST_ALGO(pred('9'));
        TEST_ALGO(!pred('a'));
    }

    // is_alnum
    {
        auto pred = is_alnum<char>();
        TEST_ALGO(pred('a'));
        TEST_ALGO(pred('5'));
        TEST_ALGO(!pred(' '));
        TEST_ALGO(!pred('!'));
    }

    // is_any_of
    {
        std::string chars = "0123456789";
        auto pred = make_is_any_of<char>(chars);
        TEST_ALGO(pred('0'));
        TEST_ALGO(pred('9'));
        TEST_ALGO(!pred('a'));
        TEST_ALGO(!pred(' '));
    }

    // is_from_range
    {
        auto pred = is_from_range<char>('a', 'z');
        TEST_ALGO(pred('a'));
        TEST_ALGO(pred('m'));
        TEST_ALGO(pred('z'));
        TEST_ALGO(!pred('A'));
        TEST_ALGO(!pred('0'));
    }

    // is_iequal
    {
        auto pred = is_iequal<char>();
        TEST_ALGO(pred('A', 'a'));
        TEST_ALGO(pred('a', 'A'));
        TEST_ALGO(pred('A', 'A'));
        TEST_ALGO(!pred('A', 'b'));
    }

    // is_less
    {
        auto pred = is_less<char>();
        TEST_ALGO(pred('a', 'b'));
        TEST_ALGO(!pred('b', 'a'));
        TEST_ALGO(!pred('a', 'a'));
    }

    // is_not_greater
    {
        auto pred = is_not_greater<char>();
        TEST_ALGO(pred('a', 'b'));
        TEST_ALGO(pred('a', 'a'));
        TEST_ALGO(!pred('b', 'a'));
    }

    std::cout << "  Classification tests passed.\n";
}

// ===========================================================================
// 9. find_iterator —— find_iterator
// ===========================================================================
void test_find_iterator() {
    TEST_SECTION("find_iterator");

    // 使用 first_finder 查找所有匹配
    {
        std::string s = "hello hello hello";
        auto finder = make_first_finder(std::string("hello"));
        auto first = find(finder, s);
        int count = 0;
        for (auto it = first; it != find_iterator<decltype(finder), decltype(s.begin()), std::string>(); ++it) {
            ++count;
        }
        TEST_ALGO(count == 3);
    }

    // 使用 last_finder
    {
        std::string s = "abcabcabc";
        auto finder = make_last_finder(std::string("abc"));
        auto first = find(finder, s);
        TEST_ALGO(!first.is_empty());
        TEST_ALGO(std::distance(first.begin(), first.end()) == 3);
        // 应找到最后一个 "abc"
        auto result = std::string(first.begin(), first.end());
        TEST_ALGO(result == "abc");
    }

    // find_iterator 哨兵
    {
        find_iterator<std::function<std::pair<std::string::const_iterator, std::string::const_iterator>(
            const std::pair<std::string::const_iterator, std::string::const_iterator>&)>,
            std::string::const_iterator, std::string> sentinel;
        TEST_ALGO(sentinel.is_empty());
    }

    std::cout << "  Find iterator tests passed.\n";
}

// ===========================================================================
// 10. 格式化器 —— formatters
// ===========================================================================
void test_formatters() {
    TEST_SECTION("formatters");

    // const_formatter
    {
        const_formatter<std::string> cf("constant");
        std::string result = cf();
        TEST_ALGO(result == "constant");
    }

    // identity_formatter
    {
        identity_formatter<std::string> ifmt;
        std::string s = "original";
        std::string result = ifmt(s);
        TEST_ALGO(result == "original");
    }

    // empty_formatter
    {
        empty_formatter<std::string> ef;
        std::string result = ef();
        TEST_ALGO(result.empty());
    }

    std::cout << "  Formatter tests passed.\n";
}

// ===========================================================================
// 11. 正则 —— regex
// ===========================================================================
void test_regex() {
    TEST_SECTION("regex");

    // replace_regex
    {
        std::string s = "hello 123 world 456";
        std::regex re("[0-9]+");
        replace_regex(s, re, std::string("NUM"));
        TEST_ALGO(s == "hello NUM world NUM");
    }
    {
        std::string s = "abc123def456ghi";
        std::regex re("[0-9]+");
        std::string r = replace_regex_copy(s, re, std::string("#"));
        TEST_ALGO(r == "abc#def#ghi");
        TEST_ALGO(s == "abc123def456ghi"); // 原串不变
    }

    // erase_regex
    {
        std::string s = "hello 123 world 456";
        std::regex re("[0-9]+");
        erase_regex(s, re);
        TEST_ALGO(s == "hello  world ");
    }
    {
        std::string s = "abc123def456ghi";
        std::regex re("[0-9]+");
        std::string r = erase_regex_copy(s, re);
        TEST_ALGO(r == "abcdefghi");
        TEST_ALGO(s == "abc123def456ghi");
    }

    // find_regex
    {
        std::string s = "abc123def";
        std::regex re("[0-9]+");
        TEST_ALGO(find_regex(s, re));
    }
    {
        std::string s = "abcdef";
        std::regex re("[0-9]+");
        TEST_ALGO(!find_regex(s, re));
    }
    {
        std::string s = "a1b2c3";
        std::regex re("[a-z]");
        TEST_ALGO(find_regex(s, re));
    }

    // split_regex
    {
        std::string s = "a1b2c3d";
        std::regex re("[0-9]+");
        std::vector<std::string> result;
        split_regex(result, s, re);
        TEST_ALGO(result.size() == 4);
        TEST_ALGO(result[0] == "a");
        TEST_ALGO(result[1] == "b");
        TEST_ALGO(result[2] == "c");
        TEST_ALGO(result[3] == "d");
    }
    {
        std::string s = "one,two,three";
        std::regex re("[,]+");
        std::vector<std::string> result;
        split_regex(result, s, re);
        TEST_ALGO(result.size() == 3);
        TEST_ALGO(result[0] == "one");
        TEST_ALGO(result[1] == "two");
        TEST_ALGO(result[2] == "three");
    }

    // find_all_regex
    {
        std::string s = "a1b22c333";
        std::regex re("[0-9]+");
        std::vector<std::pair<std::string::const_iterator, std::string::const_iterator>> result;
        find_all_regex(result, s, re);
        TEST_ALGO(result.size() == 3);
        TEST_ALGO(std::string(result[0].first, result[0].second) == "1");
        TEST_ALGO(std::string(result[1].first, result[1].second) == "22");
        TEST_ALGO(std::string(result[2].first, result[2].second) == "333");
    }

    std::cout << "  Regex tests passed.\n";
}

// ===========================================================================
// 12. find_format —— find_format
// ===========================================================================
void test_find_format() {
    TEST_SECTION("find_format");

    // 使用 first_finder + identity_formatter
    {
        std::string s = "hello world hello";
        auto finder = make_first_finder(std::string("hello"));
        std::string result = find_format_copy(finder, s, identity_formatter<std::string>());
        TEST_ALGO(result == "hello world hello");
    }

    // 使用 const_formatter 替换匹配
    {
        std::string s = "hello world hello";
        auto finder = make_first_finder(std::string("hello"));
        std::string result = find_format_copy(finder, s, const_formatter<std::string>(std::string("X")));
        TEST_ALGO(result == "X world X");
    }

    // 使用自定义 formatter
    {
        std::string s = "abc123def456";
        auto finder = make_first_finder(std::string("123"));
        std::string result = find_format_copy(finder, s,
            [](const std::string& match) {
                std::string r = "[";
                r += match;
                r += "]";
                return r;
            });
        TEST_ALGO(result == "abc[123]def456");
    }

    // find_format_all_copy
    {
        std::string s = "abc123def456";
        auto finder = make_first_finder(std::string("123"));
        std::string result = find_format_all_copy(finder, s, const_formatter<std::string>(std::string("N")));
        TEST_ALGO(result == "abcNdef456");
    }

    // find_format_all（回调方式）
    {
        std::string s = "abc123def456";
        auto finder = make_first_finder(std::string("123"));
        std::string result;
        auto formatter = [](const std::string& match) -> std::string {
            std::string r = "[";
            r += match;
            r += "]";
            return r;
        };
        auto callback = [&result](const std::string& part) {
            result += part;
        };
        find_format_all(finder, s, formatter,
            std::function<void(const std::string&)>(callback));
        TEST_ALGO(result == "abc[123]def456");
    }

    // 未找到匹配时，原始内容保留
    {
        std::string s = "no match here";
        auto finder = make_first_finder(std::string("xyz"));
        std::string result = find_format_copy(finder, s, const_formatter<std::string>(std::string("REPLACED")));
        TEST_ALGO(result == "no match here");
    }

    std::cout << "  Find format tests passed.\n";
}

// ===========================================================================
// 13. 综合测试 —— Boost 风格固定测试串
// ===========================================================================
void test_combined() {
    TEST_SECTION("combined (Boost style)");

    // Boost.Algorithm 使用的经典测试串
    {
        // "123abcxXxabcXxXabc321"
        std::string s = "123abcxXxabcXxXabc321";

        // trim
        {
            std::string t = "  123abcxXxabcXxXabc321  ";
            trim(t);
            TEST_ALGO(t == s);
        }

        // to_upper / to_lower
        {
            std::string up = s;
            to_upper(up);
            TEST_ALGO(up == "123ABCXXXABCXXXABC321");
        }
        {
            std::string low = s;
            to_lower(low);
            TEST_ALGO(low == "123abcxxxabcxxxabc321");
        }

        // replace_all
        {
            std::string r = s;
            replace_all(r, std::string("abc"), std::string("XYZ"));
            TEST_ALGO(r == "123XYZxXxXYZXxXXYZ321");
        }

        // split
        {
            std::string split_str = "123abc,xXx,abc,XxX,abc321";
            std::vector<std::string> parts;
            split(parts, split_str, [](char c) { return c == ','; }, token_compress_on);
            TEST_ALGO(parts.size() == 5);
            TEST_ALGO(parts[0] == "123abc");
            TEST_ALGO(parts[1] == "xXx");
            TEST_ALGO(parts[2] == "abc");
            TEST_ALGO(parts[3] == "XxX");
            TEST_ALGO(parts[4] == "abc321");
        }

        // find_first
        {
            TEST_ALGO(find_first(s, std::string("abc")));
            TEST_ALGO(find_first(s, std::string("XxX")));
            TEST_ALGO(!find_first(s, std::string("xyz")));
        }

        // regex
        {
            std::regex re("[0-9]+");
            TEST_ALGO(find_regex(s, re));
            std::string erased = s;
            erase_regex(erased, re);
            TEST_ALGO(erased == "abcxXxabcXxXabc");
        }
    }

    {
        // "xx-abc--xx-abb"
        std::string s = "xx-abc--xx-abb";

        // split with compress
        {
            std::vector<std::string> parts;
            split(parts, s, [](char c) { return c == '-'; }, token_compress_on);
            TEST_ALGO(parts.size() == 4);
            TEST_ALGO(parts[0] == "xx");
            TEST_ALGO(parts[1] == "abc");
            TEST_ALGO(parts[2] == "xx");
            TEST_ALGO(parts[3] == "abb");
        }
        {
            std::vector<std::string> parts;
            split(parts, s, [](char c) { return c == '-'; }, token_compress_off);
            // "xx-abc--xx-abb" 在 token_compress_off 下：
            //   "xx", "abc", "", "xx", "abb" → 共 5 个部分
            // （两个连续 '-' 之间产生一个空 token）
            TEST_ALGO(parts.size() == 5);
            TEST_ALGO(parts[0] == "xx");
            TEST_ALGO(parts[1] == "abc");
            TEST_ALGO(parts[2] == "");
            TEST_ALGO(parts[3] == "xx");
            TEST_ALGO(parts[4] == "abb");
        }

        // trim_all
        {
            std::string t = "  xx-abc--xx-abb  ";
            trim_all(t);
            TEST_ALGO(t == "xx-abc--xx-abb");
        }
    }

    {
        // "1abc3abc2"
        std::string s = "1abc3abc2";

        // replace_first / replace_all
        {
            std::string r1 = s;
            replace_first(r1, std::string("abc"), std::string("XYZ"));
            TEST_ALGO(r1 == "1XYZ3abc2");
        }
        {
            std::string r2 = s;
            replace_all(r2, std::string("abc"), std::string("XYZ"));
            TEST_ALGO(r2 == "1XYZ3XYZ2");
        }

        // erase_first / erase_all
        {
            std::string e1 = s;
            erase_first(e1, std::string("abc"));
            TEST_ALGO(e1 == "13abc2");
        }
        {
            std::string e2 = s;
            erase_all(e2, std::string("abc"));
            TEST_ALGO(e2 == "132");
        }

        // split on digits
        {
            std::vector<std::string> parts;
            split(parts, s, [](char c) { return std::isdigit(static_cast<unsigned char>(c)); },
                  token_compress_on);
            // "1abc3abc2" 以 token_compress_on 分割：
            // 前导 '1' 和尾随 '2' 都被抑制，只保留 "abc", "abc"
            TEST_ALGO(parts.size() == 2);
            TEST_ALGO(parts[0] == "abc");
            TEST_ALGO(parts[1] == "abc");
        }
    }

    std::cout << "  Combined tests passed.\n";
}

// ===========================================================================
// main
// ===========================================================================
int main() {
    std::cout << "Running string_algo tests...\n\n";

    test_case_conversion();
    test_trimming();
    test_predicates();
    test_find();
    test_replace();
    test_split();
    test_join();
    test_classification();
    test_find_iterator();
    test_formatters();
    test_regex();
    test_find_format();
    test_combined();

    std::cout << "\n================================\n";
    std::cout << "Total assertions: " << (g_pass_count + g_fail_count) << "\n";
    std::cout << "Passed:           " << g_pass_count << "\n";
    std::cout << "Failed:           " << g_fail_count << "\n";

    if (g_fail_count == 0) {
        std::cout << "\nAll tests passed!\n";
        return 0;
    } else {
        std::cout << "\nSome tests FAILED!\n";
        return 1;
    }
}
