cmake_minimum_required(VERSION 3.16)

# 项目信息
project(string_algo
    VERSION 1.0.0
    DESCRIPTION "纯标准 C++ 字符串算法库（从 Boost.Algorithm 提取）"
    LANGUAGES CXX
)

# ============================================================================
# 编译选项
# ============================================================================

# 要求 C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 默认使用 Release 构建
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# ============================================================================
# 主库（header-only）
# ============================================================================

# 创建接口库（纯头文件库无需编译源文件）
add_library(string_algo INTERFACE)

# 设置头文件搜索路径
target_include_directories(string_algo INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 要求 C++11（INTERFACE 属性对接口库有效）
target_compile_features(string_algo INTERFACE cxx_std_11)

# 源文件编码设为 UTF-8（处理中文注释）
if(MSVC)
    target_compile_options(string_algo INTERFACE /utf-8)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()


# ============================================================================
# 测试
# ============================================================================

# 仅当非子项目时启用测试
include(CTest)

if(BUILD_TESTING)
    # 主测试
    add_executable(test_string_algo tests/test_string_algo.cpp)
    target_link_libraries(test_string_algo PRIVATE string_algo)

    # 最小测试
    add_executable(min_test tests/min_test.cpp)
    target_link_libraries(min_test PRIVATE string_algo)


    # 添加测试到 ctest
    add_test(NAME test_string_algo COMMAND test_string_algo)
    add_test(NAME min_test COMMAND min_test)
endif()

# ============================================================================
# 安装
# ============================================================================

install(TARGETS string_algo EXPORT string_algoTargets)

# 安装头文件
install(DIRECTORY include/string_algo
    DESTINATION include
)

# 安装 CMake 包配置
install(EXPORT string_algoTargets
    FILE string_algoTargets.cmake
    NAMESPACE string_algo::
    DESTINATION lib/cmake/string_algo
)

# 生成 CMakeConfig 文件
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/string_algoConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# 创建 Config.cmake.in 模板
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/string_algoConfig.cmake.in
"@PACKAGE_INIT@
include(\"\${CMAKE_CURRENT_LIST_DIR}/string_algoTargets.cmake\")
check_required_components(string_algo)
")

configure_package_config_file(
    ${CMAKE_CURRENT_BINARY_DIR}/string_algoConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/string_algoConfig.cmake
    INSTALL_DESTINATION lib/cmake/string_algo
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/string_algoConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/string_algoConfigVersion.cmake
    DESTINATION lib/cmake/string_algo
)

# ============================================================================
# 打印构建信息
# ============================================================================

message(STATUS "")
message(STATUS "=== string_algo 配置完成 ===")
message(STATUS "版本:      ${PROJECT_VERSION}")
message(STATUS "构建类型:  ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ 标准:  ${CMAKE_CXX_STANDARD}")
message(STATUS "测试:      ${BUILD_TESTING}")
message(STATUS "")
