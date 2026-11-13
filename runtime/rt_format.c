/*
 * rt_format.c — Nyx Runtime Zero-Allocation Buffered Formatting Implementation
 */

#include "rt_format.h"
#include <stdio.h>
#include <string.h>

/* Fast lookup table for two-digit integer pairs (00..99) */
static const char DIGITS_LUT[200] =
    "0001020304050607080910111213141516171819"
    "2021222324252627282930313233343536373839"
    "4041424344454647484950515253545556575859"
    "6061626364656667686970717273747576777879"
    "8081828384858687888990919293949596979899";

size_t nyx_format_u64_into(uint64_t val, char* out_buf, size_t buf_len) {
    if (!out_buf || buf_len < 21) return 0;

    char temp[24];
    int pos = 23;
    temp[pos] = '\0';

    if (val == 0) {
        temp[--pos] = '0';
    } else {
        while (val >= 100) {
            uint32_t rem = (uint32_t)(val % 100);
            val /= 100;
            temp[--pos] = DIGITS_LUT[rem * 2 + 1];
            temp[--pos] = DIGITS_LUT[rem * 2];
        }
        if (val < 10) {
            temp[--pos] = (char)('0' + val);
        } else {
            temp[--pos] = DIGITS_LUT[val * 2 + 1];
            temp[--pos] = DIGITS_LUT[val * 2];
        }
    }

    size_t written = 23 - pos;
    if (written + 1 > buf_len) return 0;
    memcpy(out_buf, &temp[pos], written + 1);
    return written;
}

size_t nyx_format_i64_into(int64_t val, char* out_buf, size_t buf_len) {
    if (!out_buf || buf_len < 22) return 0;

    if (val < 0) {
        out_buf[0] = '-';
        uint64_t uval = (uint64_t)(-(val + 1)) + 1;
        size_t written = nyx_format_u64_into(uval, out_buf + 1, buf_len - 1);
        return written > 0 ? written + 1 : 0;
    } else {
        return nyx_format_u64_into((uint64_t)val, out_buf, buf_len);
    }
}

size_t nyx_format_f64_into(double val, int precision, char* out_buf, size_t buf_len) {
    if (!out_buf || buf_len < 32) return 0;
    if (precision < 0) precision = 6;
    if (precision > 12) precision = 12;

    int res = snprintf(out_buf, buf_len, "%.*f", precision, val);
    return (res > 0 && (size_t)res < buf_len) ? (size_t)res : 0;
}

bool nyx_strip_circumfix(const char* src, const char* prefix, const char* suffix, char* out_buf, size_t buf_len) {
    if (!src || !out_buf || buf_len == 0) return false;

    size_t src_len = strlen(src);
    size_t p_len = prefix ? strlen(prefix) : 0;
    size_t s_len = suffix ? strlen(suffix) : 0;

    if (src_len < p_len + s_len) return false;

    if (p_len > 0 && strncmp(src, prefix, p_len) != 0) return false;
    if (s_len > 0 && strcmp(src + src_len - s_len, suffix) != 0) return false;

    size_t inner_len = src_len - p_len - s_len;
    if (inner_len + 1 > buf_len) return false;

    memcpy(out_buf, src + p_len, inner_len);
    out_buf[inner_len] = '\0';
    return true;
}
