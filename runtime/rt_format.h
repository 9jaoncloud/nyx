/*
 * rt_format.h — Nyx Runtime Zero-Allocation Buffered Integer & Float Formatting
 *
 * Implements high-throughput, allocation-free string serialization directly
 * into user-provided stack buffers (ideal for loggers, JSON serializers, and trading engines).
 */

#ifndef RT_FORMAT_H
#define RT_FORMAT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Formats a signed 64-bit integer into out_buf. Returns number of bytes written. */
size_t nyx_format_i64_into(int64_t val, char* out_buf, size_t buf_len);

/* Formats an unsigned 64-bit integer into out_buf. Returns number of bytes written. */
size_t nyx_format_u64_into(uint64_t val, char* out_buf, size_t buf_len);

/* Formats a double-precision float with specified decimal precision. */
size_t nyx_format_f64_into(double val, int precision, char* out_buf, size_t buf_len);

/* Fast string circumfix stripper: strips matching prefix and suffix if present. */
bool nyx_strip_circumfix(const char* src, const char* prefix, const char* suffix, char* out_buf, size_t buf_len);

#ifdef __cplusplus
}
#endif

#endif /* RT_FORMAT_H */
