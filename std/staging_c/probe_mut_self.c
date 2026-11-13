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
    int64_t value;
} Counter;

Counter Counter_new();
void Counter_inc(Counter*);
int64_t Counter_get(Counter*);
int64_t nyx_main();
Counter Counter_new() {
return (Counter){.value = 0LL};
}

void Counter_inc(Counter* self) {
(self->value = ((*self).value + 1LL));
}

int64_t Counter_get(Counter* self) {
return (*self).value;
}

int64_t nyx_main() {
Counter c = Counter_new();
Counter_inc(&(c));
Counter_inc(&(c));
Counter_inc(&(c));
printf("%s%lld\n", rt_string_from("Counter: ").data, (long long)());
return 0LL;
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
