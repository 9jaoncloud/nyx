#include "runtime/rt_http.h"
#include "runtime/rt_db.h"
#include "runtime/rt_string.h"
#include "runtime/rt_arena.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
#include <stdlib.h>
#include "runtime/rt_option.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include <math.h>
#include "runtime/rt_io.h"
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

#define TAG_CalcOp_None 0
#define TAG_CalcOp_Add 1
#define TAG_CalcOp_Subtract 2
#define TAG_CalcOp_Multiply 3
#define TAG_CalcOp_Divide 4
typedef struct {
    int tag;
    union {
    } data;
} CalcOp;

typedef struct {
    int64_t display;
    int64_t previous;
    CalcOp op;
} Calculator;

Calculator Calculator_new();
void Calculator_set_value(Calculator*, int64_t);
void Calculator_set_op(Calculator*, CalcOp);
int64_t Calculator_compute(Calculator*);
void nyx_main();
Calculator Calculator_new() {
return (Calculator){.display = 0LL, .previous = 0LL, .op = (CalcOp){ .tag = TAG_CalcOp_None }};
}

void Calculator_set_value(Calculator* self, int64_t v) {
(self->display = v);
}

void Calculator_set_op(Calculator* self, CalcOp op) {
(self->previous = (*self).display);
(self->op = op);
(self->display = 0LL);
}

int64_t Calculator_compute(Calculator* self) {
int64_t current = (*self).display;
int64_t res = current;
({
int64_t __match_result_0;
CalcOp __match_val_0 = (*self).op;
if (__match_val_0.tag == TAG_CalcOp_Add) {
__match_result_0 = (res = ((*self).previous + current));
}
 else if (__match_val_0.tag == TAG_CalcOp_Subtract) {
__match_result_0 = (res = ((*self).previous - current));
}
 else if (__match_val_0.tag == TAG_CalcOp_Multiply) {
__match_result_0 = (res = ((*self).previous * current));
}
 else if (__match_val_0.tag == TAG_CalcOp_Divide) {
if ((current != 0LL)) {
__match_result_0 = (res = ((*self).previous / current));
} else {
__match_result_0 = (res = 0LL);
}
}
 else if (__match_val_0.tag == TAG_CalcOp_None) {
__match_result_0 = (res = current);
}
__match_result_0;
});
(self->display = res);
(self->op = (CalcOp){ .tag = TAG_CalcOp_None });
return res;
}

void nyx_main() {
printf("%s\n", rt_string_from("==================================================").data);
printf("%s\n", rt_string_from("🧮 NYX SCIENTIFIC CALCULATOR ENGINE (CLI RUNNER)").data);
printf("%s\n", rt_string_from("==================================================").data);
Calculator calc = Calculator_new();
Calculator_set_value(&(calc), 12LL);
Calculator_set_op(&(calc), (CalcOp){ .tag = TAG_CalcOp_Add });
Calculator_set_value(&(calc), 28LL);
int64_t sum = Calculator_compute(&(calc));
printf("%s%lld\n", rt_string_from("Calculation 1 (12 + 28) = ").data, (long long)(sum));
Calculator_set_op(&(calc), (CalcOp){ .tag = TAG_CalcOp_Multiply });
Calculator_set_value(&(calc), 5LL);
int64_t prod = Calculator_compute(&(calc));
printf("%s%lld\n", rt_string_from("Calculation 2 (40 * 5) = ").data, (long long)(prod));
Calculator_set_op(&(calc), (CalcOp){ .tag = TAG_CalcOp_Divide });
Calculator_set_value(&(calc), 4LL);
int64_t div = Calculator_compute(&(calc));
printf("%s%lld\n", rt_string_from("Calculation 3 (200 / 4) = ").data, (long long)(div));
printf("%s\n", rt_string_from("==================================================").data);
printf("%s\n", rt_string_from("✅ All calculations completed successfully!").data);
printf("%s\n", rt_string_from("==================================================").data);
}


int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
