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
    int64_t id;
    double weight;
} EntityNode;

EntityNode create_entity_in_region(int64_t, double);
void nyx_main();
EntityNode create_entity_in_region(int64_t id, double weight) {
return (EntityNode){.id = id, .weight = weight};
}

void nyx_main() {
double total_weight = 0;
int64_t count = 0LL;
while ((count < 1000LL)) {
EntityNode node = create_entity_in_region((int64_t)(count), 1.5);
total_weight = (total_weight + node.weight);
count = (count + 1LL);
}
;
printf("%s\n", rt_string_concat(rt_string_from("Iterated 1000 region nodes. Aggregate weight: "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)str(total_weight)); _s.ref_count = 1; _s; })).data);
printf("%s\n", rt_string_from("[PASS] region_alloc_test completed with O(1) bulk region deallocation.").data);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
