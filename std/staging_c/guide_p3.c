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
    rt_string_t name;
    rt_string_t email;
    int64_t age;
} User;

User User_new(rt_string_t, rt_string_t, int64_t);
rt_string_t User_to_string(User*);
int User_is_adult(User*);
int64_t nyx_main();
User User_new(rt_string_t name, rt_string_t email, int64_t age) {
return (User){.name = name, .email = email, .age = age};
}

rt_string_t User_to_string(User* self) {
return rt_string_concat(rt_string_concat(rt_string_concat((*self).name, rt_string_from(" <")), (*self).email), rt_string_from(">"));
}

int User_is_adult(User* self) {
return ((*self).age >= 18LL);
}

int64_t nyx_main() {
User u = User_new(rt_string_from("Alice"), rt_string_from("alice@example.com"), 30LL);
printf("%s\n", User_to_string(&u).data);
({ if (User_is_adult(&(u))) {
printf("%s\n", rt_string_from("adult").data);
}
});
return 0LL;
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
