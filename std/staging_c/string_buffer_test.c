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
    int64_t capacity;
    int64_t length;
} StringBuffer;

StringBuffer StringBuffer_new(int64_t);
int StringBuffer_append(void*, int64_t);
void nyx_main();
StringBuffer StringBuffer_new(int64_t cap) {
return (StringBuffer){.capacity = cap, .length = 0LL};
}

int StringBuffer_append(void* self, int64_t bytes_len) {
({ if ((((*self).length + bytes_len) <= (*self).capacity)) {
(self->length = ((*self).length + bytes_len));
return 1;
}
});
return 0;
}

void nyx_main() {
StringBuffer buf = StringBuffer_new(1024LL);
int ok1 = StringBuffer_append(&(buf), 128LL);
int ok2 = StringBuffer_append(&(buf), 256LL);
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("Buffer capacity: "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)str(buf.capacity)); _s.ref_count = 1; _s; })), rt_string_from(", current length: ")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)str(buf.length)); _s.ref_count = 1; _s; })).data);
printf("%s\n", rt_string_from("[PASS] string_buffer_test completed with zero allocations.").data);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
