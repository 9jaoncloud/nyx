#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "runtime/rt_arena.h"
#include "runtime/rt_string.h"
#include "runtime/rt_io.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_option.h"
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include "runtime/rt_concurrency.h"
#ifndef NYX_TUPLE_DEFS
#define NYX_TUPLE_DEFS
typedef struct { void* f0; void* f1; } NyxTuple2;
typedef struct { void* f0; void* f1; void* f2; } NyxTuple3;
typedef struct { void* f0; void* f1; void* f2; void* f3; } NyxTuple4;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; } NyxTuple5;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; } NyxTuple6;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; } NyxTuple7;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; void* f7; } NyxTuple8;
#endif

void println(rt_string_t);
void print(rt_string_t);
void eprintln(rt_string_t);
void eprint(rt_string_t);
void print_int(int64_t);
void print_float(double);
void println_int(int64_t);
void println_float(double);
rt_string_t format(rt_string_t);
rt_string_t format_int(int64_t);
rt_string_t format_float(double);
rt_string_t read_line();
rt_string_t read_all();
// module io
void rt_println(int64_t* p0, int64_t p1); // extern
void rt_print_int(int64_t p0); // extern
void rt_print_float(double p0); // extern
void rt_eprintln(int64_t* p0, int64_t p1); // extern
void println(rt_string_t s) {
rt_println(((char*)(s.data)), (int64_t)(s.length));
}

void print(rt_string_t s) {
rt_println(((char*)(s.data)), (int64_t)(s.length));
}

void eprintln(rt_string_t s) {
rt_eprintln(((char*)(s.data)), (int64_t)(s.length));
}

void eprint(rt_string_t s) {
rt_eprintln(((char*)(s.data)), (int64_t)(s.length));
}

void print_int(int64_t n) {
rt_print_int(n);
}

void print_float(double f) {
rt_print_float(f);
}

void println_int(int64_t n) {
rt_print_int(n);
printf("%s\n", rt_string_from("\n").data);
}

void println_float(double f) {
rt_print_float(f);
printf("%s\n", rt_string_from("\n").data);
}

rt_string_t format(rt_string_t s) {
return s;
}

rt_string_t format_int(int64_t n) {
return ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)n); _buf.ref_count = 1; _buf; });
}

rt_string_t format_float(double f) {
return ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", f); _buf.ref_count = 1; _buf; });
}

rt_string_t read_line() {
int64_t buf[4096LL] = { 0LL };
int64_t n = rt_read_stdin((&((*(int64_t*)rt_vec_get(buf, 0LL)))), 4096LL);
if ((n > 0LL)) {
return int64_t_from_utf8(String, /* slice */ 0);
} else {
return rt_string_from("");
}
}

rt_string_t read_all() {
rt_string_t result = rt_string_from("");
while (1) {
rt_string_t line = read_line();
({ if ((line.length == 0LL)) {
break;
}
});
result = rt_string_concat(rt_string_concat(result, line), rt_string_from("\n"));
}
;
return result;
}

int64_t rt_read_stdin(int64_t* p0, int64_t p1); // extern
