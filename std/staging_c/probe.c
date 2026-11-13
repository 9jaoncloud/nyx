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
#include "runtime/rt_atomic.h"
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

void show(void*, int64_t);
void nyx_main();
// module scratch.probe
// import std::ui
extern int64_t nyxos_uptime_ms();
extern rt_string_t nyxos_info_line(int64_t p0);
extern int64_t nyxos_key_pop();
void show(void* canvas, int64_t i) {
Arena* __arena = arena_create();
rt_string_t s = nyxos_info_line(i);
draw_text(canvas, s, 10, 10, 12, ({ Color _utmp_ = Color_hex(16777215LL); &_utmp_; }));
arena_destroy(__arena);
}

void nyx_main() {
Arena* __arena = arena_create();
int64_t up = nyxos_uptime_ms();
int64_t k = nyxos_key_pop();
Window window = Window_new(100LL, 100LL, rt_string_from("x"));
Canvas canvas = Window_canvas(&(window));
show((&(canvas)), 0LL);
({ if ((k > 0LL)) {
printf("%s\n", rt_string_from("k").data);
}
});
({ if ((up > 1000LL)) {
printf("%s\n", rt_string_from("up").data);
}
});
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
