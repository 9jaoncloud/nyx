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

int64_t compute_partition_hash(rt_string_t, int64_t);
MessageRecord create_record(int64_t, rt_string_t, rt_string_t, int64_t);
int64_t compute_partition_hash(rt_string_t key, int64_t num_partitions) {
int64_t len = key.length;
int64_t hash = 5381LL;
int64_t i = 0LL;
while ((i < len)) {
hash = ((((hash * 33LL) + 65LL) + i) % 1000000LL);
i = (i + 1LL);
}
;
return (hash % num_partitions);
}

void* create_record(int64_t offset, rt_string_t key, rt_string_t val, int64_t part) {
return (MessageRecord){.offset = offset, .timestamp = 1798229400LL, .key = key, .value = val, .partition_id = part};
}

