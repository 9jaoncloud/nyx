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

#define TAG_SciOp_None 0
#define TAG_SciOp_Add 1
#define TAG_SciOp_Subtract 2
#define TAG_SciOp_Multiply 3
#define TAG_SciOp_Divide 4
#define TAG_SciOp_Power 5
typedef struct {
    int tag;
    union {
    } data;
} SciOp;

typedef struct {
    double display;
    double previous;
    SciOp op;
    rt_string_t input_str;
    int entered;
} SciCalc;

double fact(int64_t);
double absf(double);
double powi(double, int64_t);
double nyx_exp(double);
double nyx_sin(double);
double nyx_cos(double);
double nyx_ln(double);
double nyx_sqrt(double);
double nyx_pow(double, double);
double nyx_tan(double);
SciCalc SciCalc_new();
void SciCalc_digit(SciCalc*, rt_string_t);
void SciCalc_dot(SciCalc*);
void SciCalc_unary(SciCalc*, rt_string_t);
void SciCalc_set_op(SciCalc*, SciOp);
void SciCalc_equals(SciCalc*);
void SciCalc_clear(SciCalc*);
int64_t nyx_main();

#line 1 "scientific_engine.nyx"
double fact(int64_t n) {
Arena* __arena = arena_create();
double r = 1;
int64_t i = 1LL;
while ((i <= n)) {
r = (r * (double)(i));
i = (i + 1LL);
}
;
arena_destroy(__arena);
return r;
}

double absf(double x) {
Arena* __arena = arena_create();
({ if ((x < 0)) {
return (-x);
}
});
arena_destroy(__arena);
return x;
}

double powi(double base, int64_t exp) {
Arena* __arena = arena_create();
double r = 1;
int64_t i = 0LL;
int neg = 0;
int64_t e = exp;
({ if ((e < 0LL)) {
neg = 1;
e = (-e);
}
});
while ((i < e)) {
r = (r * base);
i = (i + 1LL);
}
;
({ if (neg) {
({ if ((r == 0)) {
return 0;
}
});
return (1 / r);
}
});
arena_destroy(__arena);
return r;
}

double nyx_exp(double x) {
Arena* __arena = arena_create();
double sum = 1;
double term = 1;
int64_t n = 1LL;
while ((n < 40LL)) {
term = ((term * x) / (double)(n));
sum = (sum + term);
n = (n + 1LL);
({ if ((absf(term) < 0.000000000000001)) {
break;
}
});
}
;
arena_destroy(__arena);
return sum;
}

double nyx_sin(double x) {
Arena* __arena = arena_create();
double a = x;
while ((a > 3.141592653589793)) {
a = (a - 6.283185307179586);
}
;
while ((a < (-3.141592653589793))) {
a = (a + 6.283185307179586);
}
;
double sum = a;
double term = a;
int64_t n = 1LL;
while ((n < 40LL)) {
term = (-(((term * a) * a) / (((2 * (double)(n)) + 1) * (2 * (double)(n)))));
sum = (sum + term);
n = (n + 1LL);
({ if ((absf(term) < 0.000000000000001)) {
break;
}
});
}
;
arena_destroy(__arena);
return sum;
}

double nyx_cos(double x) {
Arena* __arena = arena_create();
double a = x;
while ((a > 3.141592653589793)) {
a = (a - 6.283185307179586);
}
;
while ((a < (-3.141592653589793))) {
a = (a + 6.283185307179586);
}
;
double sum = 1;
double term = 1;
int64_t n = 1LL;
while ((n < 40LL)) {
term = (-(((term * a) * a) / ((2 * (double)(n)) * ((2 * (double)(n)) - 1))));
sum = (sum + term);
n = (n + 1LL);
({ if ((absf(term) < 0.000000000000001)) {
break;
}
});
}
;
arena_destroy(__arena);
return sum;
}

double nyx_ln(double x) {
Arena* __arena = arena_create();
({ if ((x <= 0)) {
return 0;
}
});
double y = ((x - 1) / (x + 1));
double y2 = (y * y);
double sum = 0;
double term = y;
int64_t denom = 1LL;
int64_t n = 0LL;
while ((n < 200LL)) {
sum = (sum + (term / (double)(denom)));
term = (term * y2);
denom = (denom + 2LL);
n = (n + 1LL);
({ if ((absf((term / (double)(denom))) < 0.00000000000001)) {
break;
}
});
}
;
arena_destroy(__arena);
return (2 * sum);
}

double nyx_sqrt(double x) {
Arena* __arena = arena_create();
({ if ((x <= 0)) {
return 0;
}
});
({ if ((x == 1)) {
return 1;
}
});
double guess = x;
int64_t i = 0LL;
while ((i < 100LL)) {
double next = ((guess + (x / guess)) / 2);
({ if ((absf((next - guess)) < 0.0000000000001)) {
return next;
}
});
guess = next;
i = (i + 1LL);
}
;
arena_destroy(__arena);
return guess;
}

double nyx_pow(double base, double expo) {
Arena* __arena = arena_create();
({ if ((base <= 0)) {
int64_t ie = (int64_t)(expo);
({ if (((double)(ie) == expo)) {
return powi(base, ie);
}
});
return 0;
}
});
arena_destroy(__arena);
return nyx_exp((expo * nyx_ln(base)));
}

double nyx_tan(double x) {
Arena* __arena = arena_create();
double c = nyx_cos(x);
({ if ((absf(c) < 0.000000000001)) {
return 0;
}
});
arena_destroy(__arena);
return (nyx_sin(x) / c);
}

SciCalc SciCalc_new() {
return (SciCalc){.display = 0, .previous = 0, .op = (SciOp){ .tag = TAG_SciOp_None }, .input_str = rt_string_from("0"), .entered = 0};
}

void SciCalc_digit(SciCalc* self, rt_string_t d) {
double v = 0;
({ if (rt_string_eq(d, rt_string_from("1"))) {
v = 1;
} else if (rt_string_eq(d, rt_string_from("2"))) {
v = 2;
} else if (rt_string_eq(d, rt_string_from("3"))) {
v = 3;
} else if (rt_string_eq(d, rt_string_from("4"))) {
v = 4;
} else if (rt_string_eq(d, rt_string_from("5"))) {
v = 5;
} else if (rt_string_eq(d, rt_string_from("6"))) {
v = 6;
} else if (rt_string_eq(d, rt_string_from("7"))) {
v = 7;
} else if (rt_string_eq(d, rt_string_from("8"))) {
v = 8;
} else if (rt_string_eq(d, rt_string_from("9"))) {
v = 9;
}
});
({ if ((*self).entered) {
(self->display = v);
(self->input_str = d);
(self->entered = 0);
} else if (rt_string_eq((*self).input_str, rt_string_from("0"))) {
(self->display = v);
(self->input_str = d);
} else {
(self->display = (((*self).display * 10) + v));
(self->input_str = rt_string_concat((*self).input_str, d));
}
});
}

void SciCalc_dot(SciCalc* self) {
({ if ((*self).entered) {
(self->input_str = rt_string_from("0."));
(self->entered = 0);
} else if ((!rt_string_contains((*self).input_str, rt_string_from(".")))) {
(self->input_str = rt_string_concat((*self).input_str, rt_string_from(".")));
}
});
}

void SciCalc_unary(SciCalc* self, rt_string_t kind) {
double v = (*self).display;
({ if (rt_string_eq(kind, rt_string_from("sin"))) {
(self->display = nyx_sin(v));
} else if (rt_string_eq(kind, rt_string_from("cos"))) {
(self->display = nyx_cos(v));
} else if (rt_string_eq(kind, rt_string_from("tan"))) {
(self->display = nyx_tan(v));
} else if (rt_string_eq(kind, rt_string_from("ln"))) {
(self->display = nyx_ln(v));
} else if (rt_string_eq(kind, rt_string_from("log"))) {
(self->display = (nyx_ln(v) / nyx_ln(10)));
} else if (rt_string_eq(kind, rt_string_from("sqrt"))) {
(self->display = nyx_sqrt(v));
} else if (rt_string_eq(kind, rt_string_from("exp"))) {
(self->display = nyx_exp(v));
} else if (rt_string_eq(kind, rt_string_from("sqr"))) {
(self->display = (v * v));
} else if (rt_string_eq(kind, rt_string_from("inv"))) {
({ if ((v != 0)) {
(self->display = (1 / v));
} else {
(self->display = 0);
}
});
} else if (rt_string_eq(kind, rt_string_from("neg"))) {
(self->display = (-v));
} else if (rt_string_eq(kind, rt_string_from("fact"))) {
({ if (((v >= 0) && (v <= 170))) {
(self->display = fact((int64_t)(v)));
}
});
}
});
(self->input_str = ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", (*self).display); _buf.ref_count = 1; _buf; }));
(self->entered = 1);
}

void SciCalc_set_op(SciCalc* self, SciOp o) {
(self->previous = (*self).display);
(self->op = o);
(self->entered = 1);
}

void SciCalc_equals(SciCalc* self) {
double current = (*self).display;
double res = current;
({
double __match_result_21;
SciOp __match_val_21 = (*self).op;
if (__match_val_21.tag == TAG_SciOp_Add) {
__match_result_21 = (res = ((*self).previous + current));
}
 else if (__match_val_21.tag == TAG_SciOp_Subtract) {
__match_result_21 = (res = ((*self).previous - current));
}
 else if (__match_val_21.tag == TAG_SciOp_Multiply) {
__match_result_21 = (res = ((*self).previous * current));
}
 else if (__match_val_21.tag == TAG_SciOp_Divide) {
({ if ((current != 0)) {
res = ((*self).previous / current);
}
});
}
 else if (__match_val_21.tag == TAG_SciOp_Power) {
__match_result_21 = (res = nyx_pow((*self).previous, current));
}
 else if (__match_val_21.tag == TAG_SciOp_None) {
__match_result_21 = (res = current);
}
__match_result_21;
});
(self->display = res);
(self->input_str = ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", res); _buf.ref_count = 1; _buf; }));
(self->op = (SciOp){ .tag = TAG_SciOp_None });
(self->entered = 1);
}

void SciCalc_clear(SciCalc* self) {
(self->display = 0);
(self->previous = 0);
(self->op = (SciOp){ .tag = TAG_SciOp_None });
(self->input_str = rt_string_from("0"));
(self->entered = 0);
}

int64_t nyx_main() {
Arena* __arena = arena_create();
printf("%s\n", rt_string_from("=============================================").data);
printf("%s\n", rt_string_from("Sci engine self-test").data);
printf("%s\n", rt_string_from("=============================================").data);
printf("sin(0) = %f\n", nyx_sin(0));
printf("cos(0) = %f\n", nyx_cos(0));
printf("sin(pi/2) = %f\n", nyx_sin(1.5707963267948966));
printf("exp(1) = %f\n", nyx_exp(1));
printf("sqrt(2) = %f\n", nyx_sqrt(2));
printf("2^10 = %f\n", nyx_pow(2, 10));
printf("fact(5) = %f\n", fact(5LL));
SciCalc c = SciCalc_new();
SciCalc_digit(&(c), rt_string_from("1"));
SciCalc_digit(&(c), rt_string_from("2"));
SciCalc_set_op(&(c), (SciOp){ .tag = TAG_SciOp_Add });
SciCalc_digit(&(c), rt_string_from("3"));
SciCalc_digit(&(c), rt_string_from("0"));
SciCalc_equals(&(c));
printf("calc 12+30 = %f\n", c.display);
arena_destroy(__arena);
return 0LL;
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
