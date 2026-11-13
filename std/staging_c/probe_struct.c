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

typedef struct {
    int64_t x;
    int64_t y;
} Point;

Point Point_new(int64_t, int64_t);
int64_t Point_sum(void*);
int64_t nyx_main();
Point Point_new(int64_t x, int64_t y) {
return (Point){.x = x, .y = y};
}

int64_t Point_sum(void* self) {
return ((*self).x + (*self).y);
}

int64_t nyx_main() {
Point p = Point_new(3LL, 4LL);
printf("%s%lld\n", rt_string_from("Sum: ").data, (long long)(Point_sum(&(p))));
return 0LL;
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
