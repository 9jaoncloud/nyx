#include <math.h>
#include "runtime/rt_arena.h"
#include "runtime/rt_db.h"
#include "runtime/rt_io.h"
#include "runtime/rt_http.h"
#include "runtime/rt_map.h"
#include <string.h>
#include "runtime/rt_async.h"
#include <stdlib.h>
#include <stdint.h>
#include "runtime/rt_string.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_json.h"
#include <stdio.h>
#include "runtime/rt_option.h"
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
    int64_t ring_entries;
    int64_t sq_tail;
    int64_t cq_head;
    int is_kernel_polled;
} IoUringRing;

IoUringRing IoUringRing_new(int64_t);
int64_t IoUringRing_submit_io(IoUringRing*, int64_t);
// module std.net.io_uring
IoUringRing IoUringRing_new(int64_t entries) {
return (IoUringRing){.ring_entries = entries, .sq_tail = 0LL, .cq_head = 0LL, .is_kernel_polled = 1};
}

int64_t IoUringRing_submit_io(IoUringRing* self, int64_t op_code) {
if ((op_code >= 0LL)) {
return ((*self).sq_tail + 1LL);
} else {
return (*self).sq_tail;
}
}

