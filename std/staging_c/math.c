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

typedef struct {
    double x;
    double y;
} Vec2;

typedef struct {
    double x;
    double y;
    double z;
} Vec3;

typedef struct {
    double x;
    double y;
    double z;
    double w;
} Vec4;

typedef struct {
    NyxVec m;
} Matrix4;

int64_t abs_int(int64_t);
double nyx_abs(double);
double nyx_max(double, double);
double nyx_min(double, double);
int64_t max_int(int64_t, int64_t);
int64_t min_int(int64_t, int64_t);
double clamp(double, double, double);
int64_t clamp_int(int64_t, int64_t, int64_t);
double sqrt(double);
double pow(double, double);
double sin(double);
double cos(double);
double tan(double);
double asin(double);
double acos(double);
double atan(double);
double atan2(double, double);
double sinh(double);
double cosh(double);
double tanh(double);
double asinh(double);
double acosh(double);
double atanh(double);
double ln(double);
double log(double);
double log_base(double, double);
double log2(double);
double log10(double);
double log1p(double);
double exp(double);
double exp2(double);
double expm1(double);
double deg_to_rad(double);
double rad_to_deg(double);
double sigmoid(double);
double relu(double);
double norm_pdf(double);
double norm_cdf(double);
double cbrt(double);
double hypot(double, double);
double erf(double);
double erfc(double);
double tgamma(double);
double lgamma(double);
double floor(double);
double ceil(double);
double round(double);
double trunc(double);
double fmax(double, double);
double fmin(double, double);
double fdim(double, double);
double fma(double, double, double);
double fmod(double, double);
double copysign(double, double);
double nextafter(double, double);
double remainder(double, double);
int64_t gcd(int64_t, int64_t);
int64_t lcm(int64_t, int64_t);
int is_prime(int64_t);
int64_t factorial(int64_t);
int64_t binomial(int64_t, int64_t);
int is_power_of_two(int64_t);
int64_t next_power_of_two(int64_t);
double lerp(double, double, double);
int is_close(double, double, double);
double mean(NyxVec*);
double variance(NyxVec*);
double std_dev(NyxVec*);
double z_score(double, double, double);
double sign(double);
int64_t sign_int(int64_t);
Vec2 Vec2_new(double, double);
Vec2 Vec2_zero();
Vec2 Vec2_one();
double Vec2_len(Vec2*);
double Vec2_len_squared(Vec2*);
Vec2 Vec2_normalize(Vec2*);
double Vec2_dot(Vec2*, Vec2*);
double Vec2_cross(Vec2*, Vec2*);
double Vec2_distance(Vec2*, Vec2*);
Vec2 Vec2_lerp(Vec2*, Vec2*, double);
Vec2 Vec2_add(Vec2*, Vec2*);
Vec2 Vec2_sub(Vec2*, Vec2*);
Vec2 Vec2_scale(Vec2*, double);
Vec3 Vec3_new(double, double, double);
Vec3 Vec3_zero();
double Vec3_len(Vec3*);
double Vec3_len_squared(Vec3*);
Vec3 Vec3_normalize(Vec3*);
double Vec3_dot(Vec3*, Vec3*);
Vec3 Vec3_cross(Vec3*, Vec3*);
Vec3 Vec3_add(Vec3*, Vec3*);
Vec3 Vec3_sub(Vec3*, Vec3*);
Vec3 Vec3_scale(Vec3*, double);
Matrix4 Matrix4_identity();
Matrix4 Matrix4_multiply(Matrix4*, Matrix4*);
Matrix4 Matrix4_translate(double, double, double);
Matrix4 Matrix4_scale(double, double, double);
double algebraic_add(double, double);
double algebraic_sub(double, double);
double algebraic_mul(double, double);
double algebraic_div(double, double);
double algebraic_fma(double, double, double);
double algebraic_dot_product(double*, double*, int64_t);

#line 1 "math.nyx"
// module math
extern double sqrt(double);
extern double pow(double, double);
extern double sin(double);
extern double cos(double);
extern double tan(double);
extern double asin(double);
extern double acos(double);
extern double atan(double);
extern double atan2(double, double);
extern double sinh(double);
extern double cosh(double);
extern double tanh(double);
extern double asinh(double);
extern double acosh(double);
extern double atanh(double);
extern double log(double);
extern double log2(double);
extern double log10(double);
extern double exp(double);
extern double exp2(double);
extern double expm1(double);
extern double log1p(double);
extern double cbrt(double);
extern double hypot(double, double);
extern double erf(double);
extern double erfc(double);
extern double tgamma(double);
extern double lgamma(double);
extern double floor(double);
extern double ceil(double);
extern double fabs(double);
extern double fmod(double, double);
extern double round(double);
extern double trunc(double);
extern double fmax(double, double);
extern double fmin(double, double);
extern double fdim(double, double);
extern double fma(double, double, double);
extern double copysign(double, double);
extern double nextafter(double, double);
extern double remainder(double, double);
extern double remquo(double, double, int*);
int64_t abs_int(int64_t x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((x < 0LL)) {
return (-x);
} else {
return x;
}
}

double nyx_abs(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
fabs(x);
});
}

double nyx_max(double a, double b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((a > b)) {
return a;
} else {
return b;
}
}

double nyx_min(double a, double b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((a < b)) {
return a;
} else {
return b;
}
}

int64_t max_int(int64_t a, int64_t b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((a > b)) {
return a;
} else {
return b;
}
}

int64_t min_int(int64_t a, int64_t b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((a < b)) {
return a;
} else {
return b;
}
}

double clamp(double x, double lo, double hi) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return nyx_max(lo, nyx_min(x, hi));
}

int64_t clamp_int(int64_t x, int64_t lo, int64_t hi) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return max_int(lo, min_int(x, hi));
}

double sqrt(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
sqrt(x);
});
}

double pow(double base, double exp) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
pow(base, exp);
});
}

double sin(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
sin(x);
});
}

double cos(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
cos(x);
});
}

double tan(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
tan(x);
});
}

double asin(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
asin(x);
});
}

double acos(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
acos(x);
});
}

double atan(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
atan(x);
});
}

double atan2(double y, double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
atan2(y, x);
});
}

double sinh(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
sinh(x);
});
}

double cosh(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
cosh(x);
});
}

double tanh(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
tanh(x);
});
}

double asinh(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
asinh(x);
});
}

double acosh(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
acosh(x);
});
}

double atanh(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
atanh(x);
});
}

double ln(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
log(x);
});
}

double log(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
log(x);
});
}

double log_base(double x, double base) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
(log(x) / log(base));
});
}

double log2(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
log2(x);
});
}

double log10(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
log10(x);
});
}

double log1p(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
log1p(x);
});
}

double exp(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
exp(x);
});
}

double exp2(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
exp2(x);
});
}

double expm1(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
expm1(x);
});
}

double deg_to_rad(double deg) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (deg * (PI / 180));
}

double rad_to_deg(double rad) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (rad * (180 / PI));
}

double sigmoid(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (1 / (1 + exp((-x))));
}

double relu(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((x > 0)) {
return x;
} else {
return 0;
}
}

double norm_pdf(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (0.3989422804014327 * exp((-((0.5 * x) * x))));
}

double norm_cdf(double x) {
Arena* __arena = arena_create();
double abs_x = ({ double __if_res_1; if ((x >= 0)) {
__if_res_1 = x;
} else {
__if_res_1 = (-x);
}
__if_res_1; });
double k = (1 / (1 + (0.2316419 * abs_x)));
double poly = (k * (0.31938153 + (k * (-(0.356563782 + (k * (1.781477937 + (k * (-(1.821255978 + (k * 1.330274429)))))))))));
double cdf_abs = (1 - (norm_pdf(abs_x) * poly));
arena_destroy(__arena);
if ((x >= 0)) {
return cdf_abs;
} else {
return (1 - cdf_abs);
}
}

double cbrt(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
cbrt(x);
});
}

double hypot(double x, double y) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
hypot(x, y);
});
}

double erf(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
erf(x);
});
}

double erfc(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
erfc(x);
});
}

double tgamma(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
tgamma(x);
});
}

double lgamma(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
lgamma(x);
});
}

double floor(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
floor(x);
});
}

double ceil(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
ceil(x);
});
}

double round(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
round(x);
});
}

double trunc(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
trunc(x);
});
}

double fmax(double x, double y) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
fmax(x, y);
});
}

double fmin(double x, double y) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
fmin(x, y);
});
}

double fdim(double x, double y) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
fdim(x, y);
});
}

double fma(double x, double y, double z) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
fma(x, y, z);
});
}

double fmod(double x, double y) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
fmod(x, y);
});
}

double copysign(double x, double y) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
copysign(x, y);
});
}

double nextafter(double x, double y) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
nextafter(x, y);
});
}

double remainder(double x, double y) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
remainder(x, y);
});
}

int64_t gcd(int64_t a, int64_t b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((b == 0LL)) {
return abs_int(a);
} else {
return gcd(b, (a % b));
}
}

int64_t lcm(int64_t a, int64_t b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if (((a == 0LL) || (b == 0LL))) {
return 0LL;
} else {
return ((abs_int(a) / gcd(a, b)) * abs_int(b));
}
}

int is_prime(int64_t n) {
Arena* __arena = arena_create();
({ if ((n <= 1LL)) {
return 0;
}
});
({ if ((n <= 3LL)) {
return 1;
}
});
({ if ((((n % 2LL) == 0LL) || ((n % 3LL) == 0LL))) {
return 0;
}
});
int64_t i = 5LL;
while (((i * i) <= n)) {
({ if ((((n % i) == 0LL) || ((n % (i + 2LL)) == 0LL))) {
return 0;
}
});
i = (i + 6LL);
}
;
arena_destroy(__arena);
return 1;
}

int64_t factorial(int64_t n) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((n <= 1LL)) {
return 1LL;
} else {
return (n * factorial((n - 1LL)));
}
}

int64_t binomial(int64_t n, int64_t k) {
Arena* __arena = arena_create();
({ if (((k < 0LL) || (k > n))) {
return 0LL;
}
});
({ if (((k == 0LL) || (k == n))) {
return 1LL;
}
});
int64_t result = 1LL;
int64_t i = 1LL;
while ((i <= k)) {
result = ((result * ((n - k) + i)) / i);
i = (i + 1LL);
}
;
arena_destroy(__arena);
return result;
}

int is_power_of_two(int64_t n) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ((n > 0LL) && ((n & (n - 1LL)) == 0LL));
}

int64_t next_power_of_two(int64_t n) {
Arena* __arena = arena_create();
({ if ((n <= 0LL)) {
return 1LL;
}
});
int64_t v = (n - 1LL);
v = (v | (v >> 1LL));
v = (v | (v >> 2LL));
v = (v | (v >> 4LL));
v = (v | (v >> 8LL));
v = (v | (v >> 16LL));
v = (v | (v >> 32LL));
arena_destroy(__arena);
return (v + 1LL);
}

double lerp(double a, double b, double t) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (a + ((b - a) * t));
}

int is_close(double a, double b, double epsilon) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (nyx_abs((a - b)) < epsilon);
}

double mean(NyxVec* arr) {
Arena* __arena = arena_create();
int64_t n = arr->len;
({ if ((n == 0LL)) {
return 0;
}
});
double sum = 0;
int64_t i = 0LL;
while ((i < n)) {
sum = (sum + (*(int64_t*)rt_vec_get(*arr, i)));
i = (i + 1LL);
}
;
arena_destroy(__arena);
return (sum / (double)(n));
}

double variance(NyxVec* arr) {
Arena* __arena = arena_create();
int64_t n = arr->len;
({ if ((n <= 1LL)) {
return 0;
}
});
double m = mean(arr);
double sum_sq = 0;
int64_t i = 0LL;
while ((i < n)) {
double diff = ((*(int64_t*)rt_vec_get(*arr, i)) - m);
sum_sq = (sum_sq + (diff * diff));
i = (i + 1LL);
}
;
arena_destroy(__arena);
return (sum_sq / (double)((n - 1LL)));
}

double std_dev(NyxVec* arr) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return sqrt(variance(arr));
}

double z_score(double x, double mean_val, double std_val) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((std_val <= 0.000001)) {
return 0;
} else {
return ((x - mean_val) / std_val);
}
}

double sign(double x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((x > 0)) {
return 1;
} else if ((x < 0)) {
return (-1);
} else {
return 0;
}
}

int64_t sign_int(int64_t x) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((x > 0LL)) {
return 1LL;
} else if ((x < 0LL)) {
return (-1LL);
} else {
return 0LL;
}
}

Vec2 Vec2_new(double x, double y) {
return (Vec2){.x = x, .y = y};
}

Vec2 Vec2_zero() {
return (Vec2){.x = 0, .y = 0};
}

Vec2 Vec2_one() {
return (Vec2){.x = 1, .y = 1};
}

double Vec2_len(Vec2* self) {
return sqrt((((*self).x * (*self).x) + ((*self).y * (*self).y)));
}

double Vec2_len_squared(Vec2* self) {
return (((*self).x * (*self).x) + ((*self).y * (*self).y));
}

Vec2 Vec2_normalize(Vec2* self) {
int64_t l = Vec2_len(self);
({ if ((l == 0)) {
return Vec2_zero();
}
});
return (Vec2){.x = ((*self).x / l), .y = ((*self).y / l)};
}

double Vec2_dot(Vec2* self, Vec2* other) {
return (((*self).x * (*other).x) + ((*self).y * (*other).y));
}

double Vec2_cross(Vec2* self, Vec2* other) {
return (((*self).x * (*other).y) - ((*self).y * (*other).x));
}

double Vec2_distance(Vec2* self, Vec2* other) {
double dx = ((*self).x - (*other).x);
double dy = ((*self).y - (*other).y);
return sqrt(((dx * dx) + (dy * dy)));
}

Vec2 Vec2_lerp(Vec2* self, Vec2* other, double t) {
return (Vec2){.x = lerp((*self).x, (*other).x, t), .y = lerp((*self).y, (*other).y, t)};
}

Vec2 Vec2_add(Vec2* self, Vec2* other) {
return (Vec2){.x = ((*self).x + (*other).x), .y = ((*self).y + (*other).y)};
}

Vec2 Vec2_sub(Vec2* self, Vec2* other) {
return (Vec2){.x = ((*self).x - (*other).x), .y = ((*self).y - (*other).y)};
}

Vec2 Vec2_scale(Vec2* self, double s) {
return (Vec2){.x = ((*self).x * s), .y = ((*self).y * s)};
}

Vec3 Vec3_new(double x, double y, double z) {
return (Vec3){.x = x, .y = y, .z = z};
}

Vec3 Vec3_zero() {
return (Vec3){.x = 0, .y = 0, .z = 0};
}

double Vec3_len(Vec3* self) {
return sqrt(((((*self).x * (*self).x) + ((*self).y * (*self).y)) + ((*self).z * (*self).z)));
}

double Vec3_len_squared(Vec3* self) {
return ((((*self).x * (*self).x) + ((*self).y * (*self).y)) + ((*self).z * (*self).z));
}

Vec3 Vec3_normalize(Vec3* self) {
int64_t l = Vec3_len(self);
({ if ((l == 0)) {
return Vec3_zero();
}
});
return (Vec3){.x = ((*self).x / l), .y = ((*self).y / l), .z = ((*self).z / l)};
}

double Vec3_dot(Vec3* self, Vec3* other) {
return ((((*self).x * (*other).x) + ((*self).y * (*other).y)) + ((*self).z * (*other).z));
}

Vec3 Vec3_cross(Vec3* self, Vec3* other) {
return (Vec3){.x = (((*self).y * (*other).z) - ((*self).z * (*other).y)), .y = (((*self).z * (*other).x) - ((*self).x * (*other).z)), .z = (((*self).x * (*other).y) - ((*self).y * (*other).x))};
}

Vec3 Vec3_add(Vec3* self, Vec3* other) {
return (Vec3){.x = ((*self).x + (*other).x), .y = ((*self).y + (*other).y), .z = ((*self).z + (*other).z)};
}

Vec3 Vec3_sub(Vec3* self, Vec3* other) {
return (Vec3){.x = ((*self).x - (*other).x), .y = ((*self).y - (*other).y), .z = ((*self).z - (*other).z)};
}

Vec3 Vec3_scale(Vec3* self, double s) {
return (Vec3){.x = ((*self).x * s), .y = ((*self).y * s), .z = ((*self).z * s)};
}

Matrix4 Matrix4_identity() {
return (Matrix4){.m = ({ NyxVec _v = rt_vec_new(sizeof(int64_t)); double _ve12_0 = 1; rt_vec_push(&_v, &_ve12_0); double _ve12_1 = 0; rt_vec_push(&_v, &_ve12_1); double _ve12_2 = 0; rt_vec_push(&_v, &_ve12_2); double _ve12_3 = 0; rt_vec_push(&_v, &_ve12_3); double _ve12_4 = 0; rt_vec_push(&_v, &_ve12_4); double _ve12_5 = 1; rt_vec_push(&_v, &_ve12_5); double _ve12_6 = 0; rt_vec_push(&_v, &_ve12_6); double _ve12_7 = 0; rt_vec_push(&_v, &_ve12_7); double _ve12_8 = 0; rt_vec_push(&_v, &_ve12_8); double _ve12_9 = 0; rt_vec_push(&_v, &_ve12_9); double _ve12_10 = 1; rt_vec_push(&_v, &_ve12_10); double _ve12_11 = 0; rt_vec_push(&_v, &_ve12_11); double _ve12_12 = 0; rt_vec_push(&_v, &_ve12_12); double _ve12_13 = 0; rt_vec_push(&_v, &_ve12_13); double _ve12_14 = 0; rt_vec_push(&_v, &_ve12_14); double _ve12_15 = 1; rt_vec_push(&_v, &_ve12_15); _v; })};
}

Matrix4 Matrix4_multiply(Matrix4* self, Matrix4* other) {
Matrix4 result = Matrix4_identity();
for (int64_t row = 0LL; row < 4LL; row++) {
for (int64_t col = 0LL; col < 4LL; col++) {
double sum = 0;
for (int64_t k = 0LL; k < 4LL; k++) {
sum = (sum + ((*(double*)rt_vec_get((*self).m, ((row * 4LL) + k))) * (*(double*)rt_vec_get((*other).m, ((k * 4LL) + col)))));
}
;
(result.m[((row * 4LL) + col)] = sum);
}
;
}
;
return result;
}

Matrix4 Matrix4_translate(double x, double y, double z) {
Matrix4 m = Matrix4_identity();
(m.m[12LL] = x);
(m.m[13LL] = y);
(m.m[14LL] = z);
return m;
}

Matrix4 Matrix4_scale(double x, double y, double z) {
Matrix4 m = Matrix4_identity();
(m.m[0LL] = x);
(m.m[5LL] = y);
(m.m[10LL] = z);
return m;
}

// extern nyx_algebraic_add
// extern nyx_algebraic_sub
// extern nyx_algebraic_mul
// extern nyx_algebraic_div
// extern nyx_algebraic_fma
// extern nyx_algebraic_dot_product
// extern nyx_algebraic_axpy
// extern nyx_algebraic_poly_eval
double algebraic_add(double a, double b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return nyx_algebraic_add(a, b);
}

double algebraic_sub(double a, double b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return nyx_algebraic_sub(a, b);
}

double algebraic_mul(double a, double b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return nyx_algebraic_mul(a, b);
}

double algebraic_div(double a, double b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return nyx_algebraic_div(a, b);
}

double algebraic_fma(double a, double b, double c) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return nyx_algebraic_fma(a, b, c);
}

double algebraic_dot_product(double* a, double* b, int64_t len) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return nyx_algebraic_dot_product(a, b, len);
}

