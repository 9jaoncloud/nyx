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
#include "runtime/rt_net.h"
#include "runtime/rt_crypto.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_time.h"
#include "runtime/rt_nursery.h"
#include "runtime/rt_collections.h"
#include "runtime/rt_math.h"
#include "runtime/rt_format.h"
#include "runtime/rt_thread.h"
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
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

#define TAG_EscapeReason_NoEscapeLocal 0
#define TAG_EscapeReason_CallerHandoff 1
#define TAG_EscapeReason_GlobalStatic 2
#define TAG_EscapeReason_ClosureCapture 3
#define TAG_EscapeReason_CrossThreadChannel 4
typedef struct {
    int tag;
    union {
    } data;
} EscapeReason;

typedef struct {
    rt_string_t variable_name;
    int64_t source_line;
    EscapeReason reason;
    int promoted_to_arc;
} EscapeDiagnostic;

int assert_must_region(EscapeDiagnostic);
rt_string_t format_escape_warning(EscapeDiagnostic);
// module std.core.strict
int assert_must_region(EscapeDiagnostic diag) {
Arena* __arena = arena_create();
({ if (diag.promoted_to_arc) {
panic(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("COMPILER ERROR [@must_region violation]: Variable '"), diag.variable_name), rt_string_from("' escaped local region at line ")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)string(diag.source_line)); _s.ref_count = 1; _s; })));
}
});
arena_destroy(__arena);
return 1;
}

rt_string_t format_escape_warning(EscapeDiagnostic diag) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if (diag.promoted_to_arc) {
return rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("[Nyx Warn]: Variable '"), diag.variable_name), rt_string_from("' escaped region frame to Atomic ARC at line ")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)string(diag.source_line)); _s.ref_count = 1; _s; }));
} else {
return rt_string_concat(rt_string_concat(rt_string_from("[Nyx Info]: Variable '"), diag.variable_name), rt_string_from("' contained in O(1) stack region frame."));
}
}

