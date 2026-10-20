#ifndef RT_IO_H
#define RT_IO_H
#include "rt_string.h"
#include <stdint.h>

void rt_println(rt_string_t s);
void rt_print(rt_string_t s);
void rt_print_int(int64_t n);
void rt_print_float(double f);
void rt_eprintln(rt_string_t s);
int64_t rt_read_stdin(char* buf, int64_t max_len);
#endif
