#ifndef STRING_ALGO_CONSTANTS_HPP
#define STRING_ALGO_CONSTANTS_HPP

namespace string_algo {

// 标记压缩模式
enum token_compress_mode_type {
    token_compress_on,   // 压缩相邻标记
    token_compress_off  // 不压缩相邻标记
};

} // namespace string_algo

#endif // STRING_ALGO_CONSTANTS_HPP
