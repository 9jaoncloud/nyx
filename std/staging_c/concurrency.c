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

typedef struct {
    int64_t id;
    int64_t active_tasks;
    int cancelled;
} Nursery;

typedef struct {
    int64_t region_id;
    int64_t bytes_allocated;
    int64_t capacity;
} IsolateRegion;

typedef struct {
    int64_t channel_id;
    int closed;
} IsolateChannel;

int64_t* open_nursery();
void spawn_task(int64_t*, int64_t*, int64_t*);
int await_nursery(int64_t*);
int64_t* create_isolate(int64_t);
int send_isolate(int64_t*, int64_t*);
int64_t* recv_isolate(int64_t*);
// module concurrency
// import prelude::*
// extern nyx_nursery_open
// extern nyx_nursery_spawn
// extern nyx_nursery_await_all
// extern nyx_nursery_cancel
// extern nyx_nursery_close
// extern nyx_isolate_region_new
// extern nyx_isolate_region_alloc
// extern nyx_isolate_channel_new
// extern nyx_isolate_channel_send
// extern nyx_isolate_channel_recv
// extern nyx_isolate_region_free
// extern nyx_isolate_channel_free
uint8_t* open_nursery() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return nyx_nursery_open((uint8_t*)(0LL));
}

void spawn_task(int64_t* nursery, int64_t* task_fn, int64_t* arg) {
Arena* __arena = arena_create();
nyx_nursery_spawn(nursery, task_fn, arg);
arena_destroy(__arena);
}

int await_nursery(int64_t* nursery) {
Arena* __arena = arena_create();
int result = nyx_nursery_await_all(nursery);
nyx_nursery_close(nursery);
arena_destroy(__arena);
return result;
}

uint8_t* create_isolate(int64_t capacity) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return nyx_isolate_region_new(capacity);
}

int send_isolate(int64_t* ch, int64_t* region) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return nyx_isolate_channel_send(ch, region);
}

uint8_t* recv_isolate(int64_t* ch) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return nyx_isolate_channel_recv(ch);
}

