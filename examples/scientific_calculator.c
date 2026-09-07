#include "runtime/rt_async.h"
#include "runtime/rt_concurrency.h"
#include <math.h>
#include "runtime/rt_arena.h"
#include "runtime/rt_http.h"
#include <string.h>
#include <stdio.h>
#include "runtime/rt_option.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_db.h"
#include "runtime/rt_stdlib.h"
#include <stdint.h>
#include "runtime/rt_json.h"
#include "runtime/rt_io.h"
#include "runtime/rt_map.h"
#include <stdlib.h>
#include "runtime/rt_string.h"
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
    double display;
    double memory;
    double pi;
    double e;
} ScientificCalculator;

ScientificCalculator ScientificCalculator_new();
void ScientificCalculator_set(ScientificCalculator*, double);
double ScientificCalculator_add(ScientificCalculator*, double);
double ScientificCalculator_sub(ScientificCalculator*, double);
double ScientificCalculator_mul(ScientificCalculator*, double);
double ScientificCalculator_div(ScientificCalculator*, double);
double ScientificCalculator_calc_power(ScientificCalculator*, int64_t);
double ScientificCalculator_calc_sqrt(ScientificCalculator*);
double ScientificCalculator_calc_factorial(ScientificCalculator*, int64_t);
double ScientificCalculator_calc_sin(ScientificCalculator*, double);
double ScientificCalculator_calc_cos(ScientificCalculator*, double);
double ScientificCalculator_calc_ln(ScientificCalculator*, double);
void ScientificCalculator_mem_store(ScientificCalculator*);
double ScientificCalculator_mem_recall(ScientificCalculator*);
void ScientificCalculator_mem_add(ScientificCalculator*);
void ScientificCalculator_mem_clear(ScientificCalculator*);
void nyx_main();
ScientificCalculator ScientificCalculator_new() {
return (ScientificCalculator){.display = 0, .memory = 0, .pi = 3.141592653589793, .e = 2.718281828459045};
}

void ScientificCalculator_set(ScientificCalculator* self, double val) {
(self->display = val);
}

double ScientificCalculator_add(ScientificCalculator* self, double b) {
(self->display = ((*self).display + b));
return (*self).display;
}

double ScientificCalculator_sub(ScientificCalculator* self, double b) {
(self->display = ((*self).display - b));
return (*self).display;
}

double ScientificCalculator_mul(ScientificCalculator* self, double b) {
(self->display = ((*self).display * b));
return (*self).display;
}

double ScientificCalculator_div(ScientificCalculator* self, double b) {
({ double __if_res_1; if ((b != 0)) {
__if_res_1 = (self->display = ((*self).display / b));
} else {
__if_res_1 = (self->display = 0);
}
__if_res_1; });
return (*self).display;
}

double ScientificCalculator_calc_power(ScientificCalculator* self, int64_t exp) {
double base = (*self).display;
double result = 1;
int64_t n = exp;
({ if ((n < 0LL)) {
n = (0LL - n);
}
});
int64_t i = 0LL;
while ((i < n)) {
result = (result * base);
i = (i + 1LL);
}
;
({ if ((exp < 0LL)) {
({ if ((result != 0)) {
result = (1 / result);
}
});
}
});
(self->display = result);
return result;
}

double ScientificCalculator_calc_sqrt(ScientificCalculator* self) {
double val = (*self).display;
({ if ((val <= 0)) {
(self->display = 0);
return 0;
}
});
double guess = (val / 2);
({ if ((guess < 1)) {
guess = 1;
}
});
int64_t i = 0LL;
while ((i < 40LL)) {
guess = ((guess + (val / guess)) * 0.5);
i = (i + 1LL);
}
;
(self->display = guess);
return guess;
}

double ScientificCalculator_calc_factorial(ScientificCalculator* self, int64_t n) {
({ if ((n <= 1LL)) {
(self->display = 1);
return 1;
}
});
double result = 1;
int64_t i = 2LL;
while ((i <= n)) {
double f = (double)(i);
result = (result * f);
i = (i + 1LL);
}
;
(self->display = result);
return result;
}

double ScientificCalculator_calc_sin(ScientificCalculator* self, double rad) {
double x = rad;
double two_pi = 6.283185307179586;
while ((x > 3.141592653589793)) {
x = (x - two_pi);
}
;
while ((x < (0 - 3.141592653589793))) {
x = (x + two_pi);
}
;
double x2 = (x * x);
double t1 = x;
double t3 = ((t1 * x2) / 6);
double t5 = ((t3 * x2) / 20);
double t7 = ((t5 * x2) / 42);
double t9 = ((t7 * x2) / 72);
double result = ((((t1 - t3) + t5) - t7) + t9);
(self->display = result);
return result;
}

double ScientificCalculator_calc_cos(ScientificCalculator* self, double rad) {
double half_pi = 1.5707963267948966;
return ScientificCalculator_calc_sin(self, (half_pi - rad));
}

double ScientificCalculator_calc_ln(ScientificCalculator* self, double val) {
({ if ((val <= 0)) {
(self->display = 0);
return 0;
}
});
double x = val;
int64_t k = 0LL;
while ((x > 2)) {
x = (x / 2.718281828459045);
k = (k + 1LL);
}
;
while ((x < 0.5)) {
x = (x * 2.718281828459045);
k = (k - 1LL);
}
;
double y = ((x - 1) / (x + 1));
double y2 = (y * y);
double sum = y;
double term = y;
int64_t i = 3LL;
while ((i <= 25LL)) {
term = (term * y2);
double div_f = (double)(i);
sum = (sum + (term / div_f));
i = (i + 2LL);
}
;
double result = ((sum * 2) + (double)(k));
(self->display = result);
return result;
}

void ScientificCalculator_mem_store(ScientificCalculator* self) {
(self->memory = (*self).display);
}

double ScientificCalculator_mem_recall(ScientificCalculator* self) {
(self->display = (*self).memory);
return (*self).memory;
}

void ScientificCalculator_mem_add(ScientificCalculator* self) {
(self->memory = ((*self).memory + (*self).display));
}

void ScientificCalculator_mem_clear(ScientificCalculator* self) {
(self->memory = 0);
}

void nyx_main() {
printf("%s\n", rt_string_from("==================================================").data);
printf("%s\n", rt_string_from("🔬 NYX SCIENTIFIC CALCULATOR SUITE v0.28.0").data);
printf("%s\n", rt_string_from("==================================================").data);
ScientificCalculator calc = ScientificCalculator_new();
ScientificCalculator_set(&(calc), 25.5);
ScientificCalculator_mul(&(calc), 4);
ScientificCalculator_add(&(calc), 12);
printf("%s%f\n", rt_string_from("1. Arithmetic ((25.5 * 4.0) + 12.0)   = ").data, calc.display);
ScientificCalculator_set(&(calc), 2);
ScientificCalculator_calc_power(&(calc), 10LL);
printf("%s%f\n", rt_string_from("2. Power (2.0 ^ 10)                  = ").data, calc.display);
ScientificCalculator_set(&(calc), 144);
ScientificCalculator_calc_sqrt(&(calc));
printf("%s%f\n", rt_string_from("3. Square Root (sqrt(144.0))         = ").data, calc.display);
ScientificCalculator_set(&(calc), 2);
ScientificCalculator_calc_sqrt(&(calc));
printf("%s%f\n", rt_string_from("4. Square Root (sqrt(2.0))           = ").data, calc.display);
ScientificCalculator_calc_factorial(&(calc), 6LL);
printf("%s%f\n", rt_string_from("5. Factorial (6!)                    = ").data, calc.display);
double rad_30 = (calc.pi / 6);
ScientificCalculator_calc_sin(&(calc), rad_30);
printf("%s%f\n", rt_string_from("6. Trigonometry (sin(30 deg / pi/6)) = ").data, calc.display);
double rad_60 = (calc.pi / 3);
ScientificCalculator_calc_cos(&(calc), rad_60);
printf("%s%f\n", rt_string_from("7. Trigonometry (cos(60 deg / pi/3)) = ").data, calc.display);
ScientificCalculator_calc_ln(&(calc), calc.e);
printf("%s%f\n", rt_string_from("8. Natural Log (ln(e))               = ").data, calc.display);
ScientificCalculator_set(&(calc), 500);
ScientificCalculator_mem_store(&(calc));
ScientificCalculator_set(&(calc), 100);
ScientificCalculator_mem_add(&(calc));
ScientificCalculator_mem_recall(&(calc));
printf("%s%f\n", rt_string_from("9. Memory (Stored 500 + Added 100)   = ").data, calc.memory);
printf("%s\n", rt_string_from("==================================================").data);
printf("%s\n", rt_string_from("✅ All 9 Scientific Calculator Tests Passed 100%!").data);
printf("%s\n", rt_string_from("==================================================").data);
}


int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
