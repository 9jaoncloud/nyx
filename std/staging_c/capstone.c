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
#include "runtime/nyx_generational.h"
#include "include/nyx_multidialect.h"
/* Odin-inspired Native Execution Context */
typedef struct NyxContext {
    void* nursery;
    void* temp_nursery;
    uint64_t thread_id;
    void (*logger)(const char* msg);
    void* user_data;
} NyxContext;
#if defined(_MSC_VER)
__declspec(thread) NyxContext nyx_current_context = {0};
#else
__thread NyxContext nyx_current_context = {0};
#endif
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

#define TAG_Movement_Credit 0
#define TAG_Movement_Debit 1
typedef struct {
    int tag;
    union {
        struct { int64_t f0; } Credit;
        struct { int64_t f0; } Debit;
    } data;
} Movement;

typedef struct {
    rt_string_t owner;
    int64_t balance;
} Account;

Account Account_new(rt_string_t, int64_t);
void Account_deposit(Account*, int64_t);
int Account_withdraw(Account*, int64_t);
rt_string_t movement_label(Movement);
void nyx_main();

#line 1 "capstone.nyx"
Account Account_new(rt_string_t owner, int64_t opening) {
return (Account){.owner = owner, .balance = opening};
}

void Account_deposit(Account* self, int64_t amount) {
(self->balance = ((*self).balance + amount));
}

int Account_withdraw(Account* self, int64_t amount) {
if ((amount > (*self).balance)) {
return 0;
} else {
(self->balance = ((*self).balance - amount));
return 1;
}
}

rt_string_t movement_label(Movement m) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_0;
Movement __match_val_0 = m;
if (__match_val_0.tag == TAG_Movement_Credit) {
    int64_t v = __match_val_0.data.Credit.f0;
__match_result_0 = rt_string_concat(rt_string_from("CREDIT "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)v); _buf.ref_count = 1; _buf; }));
}
 else if (__match_val_0.tag == TAG_Movement_Debit) {
    int64_t v = __match_val_0.data.Debit.f0;
__match_result_0 = rt_string_concat(rt_string_from("DEBIT  "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)v); _buf.ref_count = 1; _buf; }));
}
__match_result_0;
});
}

void nyx_main() {
Arena* __arena = arena_create();
printf("%s\n", rt_string_from("=== NYX LEDGER ===").data);
Account acc = Account_new(rt_string_from("Ada"), 1000LL);
NyxVec credits = ({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve1_0 = 500LL; rt_vec_push(&_v, &_ve1_0); int64_t _ve1_1 = 250LL; rt_vec_push(&_v, &_ve1_1); int64_t _ve1_2 = 100LL; rt_vec_push(&_v, &_ve1_2); _v; });
int64_t i = 0LL;
while ((i < credits.len)) {
Account_deposit(&(acc), (*(int64_t*)rt_vec_get(credits, i)));
printf("credited %lld\n", (long long)((*(int64_t*)rt_vec_get(credits, i))));
i = (i + 1LL);
}
;
int ok = Account_withdraw(&(acc), 1000LL);
assert_true((ok == 1));
int over = Account_withdraw(&(acc), 999999LL);
assert_true((over == 0));
printf("%s\n", rt_string_concat(rt_string_from("label: "), movement_label((Movement){ .tag = TAG_Movement_Credit, .data.Credit = {.f0 = 50LL} })).data);
printf("%s\n", rt_string_concat(rt_string_from("final balance: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)acc.balance); _buf.ref_count = 1; _buf; })).data);
assert_true((acc.balance == 850LL));
printf("%s\n", rt_string_from("=== DONE ===").data);
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
