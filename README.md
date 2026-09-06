# string_algo

纯标准 C++11 字符串算法库，从 Boost.Algorithm 字符串算法提取，仅依赖 C++ 标准库，无任何第三方依赖。

## 特性

- **大小写转换** — `to_upper` / `to_lower`（含 `_copy` 变体）
- **修剪** — `trim` / `trim_left` / `trim_right` / `trim_all`
- **谓词** — `starts_with` / `ends_with` / `contains` / `equals`
- **查找算法** — `find_first` / `find_last` / `find_nth` / `find_head` / `find_tail`
- **替换 / 删除** — `replace_first` / `replace_all` / `erase_first` / `erase_all`
- **分割** — `split` / `find_all`
- **连接** — `join` / `join_if`
- **分类** — `is_space` / `is_alpha` / `is_digit` 等
- **查找器** — `first_finder` / `last_finder` / `nth_finder` / `head_finder` / `tail_finder`
- **格式化器** — `const_formatter` / `identity_formatter` / `empty_formatter`
- **迭代器** — `find_iterator` / `split_iterator`
- **正则支持** — `find_regex` / `replace_regex` / `erase_regex` / `split_regex`（基于 `std::regex`）

## 用法

```cpp
#include <string_algo/string.hpp>
using namespace string_algo;

std::string s = " hello world! ";
trim(s);                          // s == "hello world!"
to_upper(s);                      // s == "HELLO WORLD!"
replace_all(s, "WORLD", "STANDARD C++"); // s == "HELLO STANDARD C++!"

// 正则示例：移除所有数字
std::regex re("[0-9]+");
erase_regex(s, re);
```

## 构建

### 依赖

- CMake 3.16+
- C++20 编译器（MSVC 2022+ / GCC 11+ / Clang 14+）

### 编译测试

```bash
# 配置
cmake -B build

# 构建
cmake --build build --config Release

# 运行测试
cd build && ctest --output-on-failure
```

### 作为库使用

#### 方式一：直接包含头文件

```cpp
#include <string_algo/string.hpp>
```

编译时添加 `-I/path/to/string_algo/include`。

#### 方式二：CMake FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    string_algo
    GIT_REPOSITORY https://github.com/qq1134993111/string_algo.git
)
FetchContent_MakeAvailable(string_algo)

target_link_libraries(your_target PRIVATE string_algo::string_algo)
```

#### 方式三：install / find_package

```bash
cmake --build build --target install
```

```cmake
find_package(string_algo REQUIRED)
target_link_libraries(your_target PRIVATE string_algo::string_algo)
```

## 项目结构

```
string_algo/
├── CMakeLists.txt
├── include/
│   └── string_algo/
│       ├── string.hpp          # 主入口头文件
│       └── string/
│           ├── case_conv.hpp   # 大小写转换
│           ├── classification.hpp
│           ├── config.hpp
│           ├── constants.hpp
│           ├── find.hpp
│           ├── find_format.hpp
│           ├── find_iterator.hpp
│           ├── find_regex.hpp
│           ├── finder.hpp
│           ├── join.hpp
│           ├── predicate.hpp
│           ├── replace.hpp
│           ├── replace_regex.hpp
│           ├── split.hpp
│           ├── split_regex.hpp
│           └── trim.hpp
└── tests/
    ├── test_string_algo.cpp    # 主测试
    ├── min_test.cpp            # 最小测试
    └── test_include.cpp        # 头文件包含测试
```

## 许可证

MIT License
