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

typedef struct {
    double bias;
    double w0;
    double w1;
    double w2;
    double w3;
    double w4;
} RoboticsPhysicalTempLearned;

RoboticsPhysicalTempLearned RoboticsPhysicalTempLearned_new();
double RoboticsPhysicalTempLearned_execute_step(RoboticsPhysicalTempLearned*, double, double, double, double, double);
void nyx_main();
RoboticsPhysicalTempLearned RoboticsPhysicalTempLearned_new() {
return (RoboticsPhysicalTempLearned){.bias = (-8.150931909), .w0 = 1.6921163129, .w1 = (-0.7022771745), .w2 = 0.0073881181, .w3 = 0.0112599042, .w4 = (-0.0028616768)};
}

double RoboticsPhysicalTempLearned_execute_step(RoboticsPhysicalTempLearned* self, double f0, double f1, double f2, double f3, double f4) {
double s = ((((((*self).bias + ((*self).w0 * f0)) + ((*self).w1 * f1)) + ((*self).w2 * f2)) + ((*self).w3 * f3)) + ((*self).w4 * f4));
return s;
}

void nyx_main() {
Arena* __arena = arena_create();
RoboticsPhysicalTempLearned s0 = RoboticsPhysicalTempLearned_new();
double r0 = RoboticsPhysicalTempLearned_execute_step((&(s0)), 14.4, 13.7, 1019.6, 90, 2.9);
int64_t sc0 = (int64_t)((r0 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc0); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s1 = RoboticsPhysicalTempLearned_new();
double r1 = RoboticsPhysicalTempLearned_execute_step((&(s1)), 15.5, 14.4, 1019.2, 86, 1.8);
int64_t sc1 = (int64_t)((r1 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc1); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s2 = RoboticsPhysicalTempLearned_new();
double r2 = RoboticsPhysicalTempLearned_execute_step((&(s2)), 17.1, 15.5, 1018.6, 79, 4);
int64_t sc2 = (int64_t)((r2 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc2); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s3 = RoboticsPhysicalTempLearned_new();
double r3 = RoboticsPhysicalTempLearned_execute_step((&(s3)), 18.4, 17.1, 1018, 73, 1.8);
int64_t sc3 = (int64_t)((r3 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc3); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s4 = RoboticsPhysicalTempLearned_new();
double r4 = RoboticsPhysicalTempLearned_execute_step((&(s4)), 19.6, 18.4, 1017.5, 66, 1.4);
int64_t sc4 = (int64_t)((r4 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc4); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s5 = RoboticsPhysicalTempLearned_new();
double r5 = RoboticsPhysicalTempLearned_execute_step((&(s5)), 20.2, 19.6, 1016.8, 63, 0.4);
int64_t sc5 = (int64_t)((r5 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc5); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s6 = RoboticsPhysicalTempLearned_new();
double r6 = RoboticsPhysicalTempLearned_execute_step((&(s6)), 20.8, 20.2, 1016.1, 60, 2.9);
int64_t sc6 = (int64_t)((r6 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc6); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s7 = RoboticsPhysicalTempLearned_new();
double r7 = RoboticsPhysicalTempLearned_execute_step((&(s7)), 21.1, 20.8, 1015.9, 56, 2.9);
int64_t sc7 = (int64_t)((r7 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc7); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s8 = RoboticsPhysicalTempLearned_new();
double r8 = RoboticsPhysicalTempLearned_execute_step((&(s8)), 21.3, 21.1, 1015.6, 54, 4.3);
int64_t sc8 = (int64_t)((r8 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc8); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s9 = RoboticsPhysicalTempLearned_new();
double r9 = RoboticsPhysicalTempLearned_execute_step((&(s9)), 21, 21.3, 1015.4, 58, 6.5);
int64_t sc9 = (int64_t)((r9 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc9); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s10 = RoboticsPhysicalTempLearned_new();
double r10 = RoboticsPhysicalTempLearned_execute_step((&(s10)), 20.1, 21, 1015.4, 67, 9.4);
int64_t sc10 = (int64_t)((r10 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc10); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s11 = RoboticsPhysicalTempLearned_new();
double r11 = RoboticsPhysicalTempLearned_execute_step((&(s11)), 19.2, 20.1, 1015, 73, 9);
int64_t sc11 = (int64_t)((r11 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc11); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s12 = RoboticsPhysicalTempLearned_new();
double r12 = RoboticsPhysicalTempLearned_execute_step((&(s12)), 18.5, 19.2, 1014.6, 79, 6.8);
int64_t sc12 = (int64_t)((r12 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc12); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s13 = RoboticsPhysicalTempLearned_new();
double r13 = RoboticsPhysicalTempLearned_execute_step((&(s13)), 18, 18.5, 1014.5, 82, 6.5);
int64_t sc13 = (int64_t)((r13 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc13); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s14 = RoboticsPhysicalTempLearned_new();
double r14 = RoboticsPhysicalTempLearned_execute_step((&(s14)), 17.9, 18, 1014.1, 81, 5.8);
int64_t sc14 = (int64_t)((r14 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc14); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s15 = RoboticsPhysicalTempLearned_new();
double r15 = RoboticsPhysicalTempLearned_execute_step((&(s15)), 17.5, 17.9, 1013.9, 82, 5.8);
int64_t sc15 = (int64_t)((r15 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc15); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s16 = RoboticsPhysicalTempLearned_new();
double r16 = RoboticsPhysicalTempLearned_execute_step((&(s16)), 16.9, 17.5, 1013.9, 86, 5);
int64_t sc16 = (int64_t)((r16 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc16); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s17 = RoboticsPhysicalTempLearned_new();
double r17 = RoboticsPhysicalTempLearned_execute_step((&(s17)), 16.6, 16.9, 1013, 87, 5.8);
int64_t sc17 = (int64_t)((r17 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc17); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s18 = RoboticsPhysicalTempLearned_new();
double r18 = RoboticsPhysicalTempLearned_execute_step((&(s18)), 16.3, 16.6, 1012.6, 90, 4.7);
int64_t sc18 = (int64_t)((r18 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc18); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s19 = RoboticsPhysicalTempLearned_new();
double r19 = RoboticsPhysicalTempLearned_execute_step((&(s19)), 16.3, 16.3, 1012, 90, 4.7);
int64_t sc19 = (int64_t)((r19 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc19); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s20 = RoboticsPhysicalTempLearned_new();
double r20 = RoboticsPhysicalTempLearned_execute_step((&(s20)), 16, 16.3, 1011.6, 90, 7.2);
int64_t sc20 = (int64_t)((r20 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc20); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s21 = RoboticsPhysicalTempLearned_new();
double r21 = RoboticsPhysicalTempLearned_execute_step((&(s21)), 15.9, 16, 1011.8, 90, 9.4);
int64_t sc21 = (int64_t)((r21 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc21); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s22 = RoboticsPhysicalTempLearned_new();
double r22 = RoboticsPhysicalTempLearned_execute_step((&(s22)), 15.8, 15.9, 1011.9, 89, 11.9);
int64_t sc22 = (int64_t)((r22 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc22); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s23 = RoboticsPhysicalTempLearned_new();
double r23 = RoboticsPhysicalTempLearned_execute_step((&(s23)), 15.2, 15.8, 1012.1, 87, 15.1);
int64_t sc23 = (int64_t)((r23 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc23); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s24 = RoboticsPhysicalTempLearned_new();
double r24 = RoboticsPhysicalTempLearned_execute_step((&(s24)), 14.3, 15.2, 1012.5, 91, 10.4);
int64_t sc24 = (int64_t)((r24 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc24); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s25 = RoboticsPhysicalTempLearned_new();
double r25 = RoboticsPhysicalTempLearned_execute_step((&(s25)), 13.8, 14.3, 1012.1, 94, 10.4);
int64_t sc25 = (int64_t)((r25 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc25); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s26 = RoboticsPhysicalTempLearned_new();
double r26 = RoboticsPhysicalTempLearned_execute_step((&(s26)), 14.2, 13.8, 1011.1, 92, 14);
int64_t sc26 = (int64_t)((r26 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc26); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s27 = RoboticsPhysicalTempLearned_new();
double r27 = RoboticsPhysicalTempLearned_execute_step((&(s27)), 14.1, 14.2, 1011.4, 89, 13.7);
int64_t sc27 = (int64_t)((r27 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc27); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s28 = RoboticsPhysicalTempLearned_new();
double r28 = RoboticsPhysicalTempLearned_execute_step((&(s28)), 13.4, 14.1, 1011.7, 90, 13.3);
int64_t sc28 = (int64_t)((r28 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc28); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s29 = RoboticsPhysicalTempLearned_new();
double r29 = RoboticsPhysicalTempLearned_execute_step((&(s29)), 12.8, 13.4, 1011.6, 92, 16.2);
int64_t sc29 = (int64_t)((r29 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc29); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s30 = RoboticsPhysicalTempLearned_new();
double r30 = RoboticsPhysicalTempLearned_execute_step((&(s30)), 12.2, 12.8, 1011.7, 92, 17.3);
int64_t sc30 = (int64_t)((r30 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc30); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s31 = RoboticsPhysicalTempLearned_new();
double r31 = RoboticsPhysicalTempLearned_execute_step((&(s31)), 11.7, 12.2, 1012.2, 88, 17.3);
int64_t sc31 = (int64_t)((r31 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc31); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s32 = RoboticsPhysicalTempLearned_new();
double r32 = RoboticsPhysicalTempLearned_execute_step((&(s32)), 11.6, 11.7, 1012.6, 85, 15.5);
int64_t sc32 = (int64_t)((r32 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc32); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s33 = RoboticsPhysicalTempLearned_new();
double r33 = RoboticsPhysicalTempLearned_execute_step((&(s33)), 11.6, 11.6, 1013.1, 78, 13);
int64_t sc33 = (int64_t)((r33 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc33); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s34 = RoboticsPhysicalTempLearned_new();
double r34 = RoboticsPhysicalTempLearned_execute_step((&(s34)), 11.3, 11.6, 1013.4, 81, 8.6);
int64_t sc34 = (int64_t)((r34 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc34); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s35 = RoboticsPhysicalTempLearned_new();
double r35 = RoboticsPhysicalTempLearned_execute_step((&(s35)), 11.1, 11.3, 1014.1, 82, 7.6);
int64_t sc35 = (int64_t)((r35 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc35); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s36 = RoboticsPhysicalTempLearned_new();
double r36 = RoboticsPhysicalTempLearned_execute_step((&(s36)), 10.7, 11.1, 1014.8, 75, 11.2);
int64_t sc36 = (int64_t)((r36 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc36); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s37 = RoboticsPhysicalTempLearned_new();
double r37 = RoboticsPhysicalTempLearned_execute_step((&(s37)), 10.2, 10.7, 1015.1, 73, 14.4);
int64_t sc37 = (int64_t)((r37 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc37); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s38 = RoboticsPhysicalTempLearned_new();
double r38 = RoboticsPhysicalTempLearned_execute_step((&(s38)), 9.7, 10.2, 1015.9, 75, 14.4);
int64_t sc38 = (int64_t)((r38 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc38); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s39 = RoboticsPhysicalTempLearned_new();
double r39 = RoboticsPhysicalTempLearned_execute_step((&(s39)), 9.4, 9.7, 1016.1, 75, 13);
int64_t sc39 = (int64_t)((r39 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc39); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s40 = RoboticsPhysicalTempLearned_new();
double r40 = RoboticsPhysicalTempLearned_execute_step((&(s40)), 9, 9.4, 1016.2, 78, 12.2);
int64_t sc40 = (int64_t)((r40 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc40); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s41 = RoboticsPhysicalTempLearned_new();
double r41 = RoboticsPhysicalTempLearned_execute_step((&(s41)), 8.7, 9, 1017, 79, 14);
int64_t sc41 = (int64_t)((r41 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc41); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s42 = RoboticsPhysicalTempLearned_new();
double r42 = RoboticsPhysicalTempLearned_execute_step((&(s42)), 8.6, 8.7, 1017.2, 80, 13);
int64_t sc42 = (int64_t)((r42 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc42); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s43 = RoboticsPhysicalTempLearned_new();
double r43 = RoboticsPhysicalTempLearned_execute_step((&(s43)), 8.3, 8.6, 1017.8, 80, 10.1);
int64_t sc43 = (int64_t)((r43 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc43); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s44 = RoboticsPhysicalTempLearned_new();
double r44 = RoboticsPhysicalTempLearned_execute_step((&(s44)), 8, 8.3, 1018.3, 81, 10.1);
int64_t sc44 = (int64_t)((r44 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc44); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s45 = RoboticsPhysicalTempLearned_new();
double r45 = RoboticsPhysicalTempLearned_execute_step((&(s45)), 7.9, 8, 1018.7, 81, 10.4);
int64_t sc45 = (int64_t)((r45 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc45); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s46 = RoboticsPhysicalTempLearned_new();
double r46 = RoboticsPhysicalTempLearned_execute_step((&(s46)), 8, 7.9, 1019.5, 79, 11.5);
int64_t sc46 = (int64_t)((r46 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc46); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s47 = RoboticsPhysicalTempLearned_new();
double r47 = RoboticsPhysicalTempLearned_execute_step((&(s47)), 8.9, 8, 1020.6, 73, 14.8);
int64_t sc47 = (int64_t)((r47 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc47); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s48 = RoboticsPhysicalTempLearned_new();
double r48 = RoboticsPhysicalTempLearned_execute_step((&(s48)), 10.1, 8.9, 1021.3, 67, 11.2);
int64_t sc48 = (int64_t)((r48 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc48); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s49 = RoboticsPhysicalTempLearned_new();
double r49 = RoboticsPhysicalTempLearned_execute_step((&(s49)), 11.5, 10.1, 1021.7, 60, 11.5);
int64_t sc49 = (int64_t)((r49 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc49); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s50 = RoboticsPhysicalTempLearned_new();
double r50 = RoboticsPhysicalTempLearned_execute_step((&(s50)), 12.6, 11.5, 1021.7, 52, 11.9);
int64_t sc50 = (int64_t)((r50 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc50); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s51 = RoboticsPhysicalTempLearned_new();
double r51 = RoboticsPhysicalTempLearned_execute_step((&(s51)), 13.9, 12.6, 1022.2, 44, 13.7);
int64_t sc51 = (int64_t)((r51 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc51); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s52 = RoboticsPhysicalTempLearned_new();
double r52 = RoboticsPhysicalTempLearned_execute_step((&(s52)), 14.7, 13.9, 1022, 41, 14);
int64_t sc52 = (int64_t)((r52 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc52); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s53 = RoboticsPhysicalTempLearned_new();
double r53 = RoboticsPhysicalTempLearned_execute_step((&(s53)), 15.3, 14.7, 1022.1, 37, 14.4);
int64_t sc53 = (int64_t)((r53 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc53); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s54 = RoboticsPhysicalTempLearned_new();
double r54 = RoboticsPhysicalTempLearned_execute_step((&(s54)), 15.5, 15.3, 1022.1, 38, 13);
int64_t sc54 = (int64_t)((r54 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc54); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s55 = RoboticsPhysicalTempLearned_new();
double r55 = RoboticsPhysicalTempLearned_execute_step((&(s55)), 15.2, 15.5, 1022.1, 42, 13.3);
int64_t sc55 = (int64_t)((r55 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc55); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s56 = RoboticsPhysicalTempLearned_new();
double r56 = RoboticsPhysicalTempLearned_execute_step((&(s56)), 14.7, 15.2, 1022, 46, 11.2);
int64_t sc56 = (int64_t)((r56 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc56); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s57 = RoboticsPhysicalTempLearned_new();
double r57 = RoboticsPhysicalTempLearned_execute_step((&(s57)), 14.2, 14.7, 1021.9, 51, 7.6);
int64_t sc57 = (int64_t)((r57 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc57); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s58 = RoboticsPhysicalTempLearned_new();
double r58 = RoboticsPhysicalTempLearned_execute_step((&(s58)), 14.2, 14.2, 1021.8, 53, 8.6);
int64_t sc58 = (int64_t)((r58 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc58); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s59 = RoboticsPhysicalTempLearned_new();
double r59 = RoboticsPhysicalTempLearned_execute_step((&(s59)), 13.6, 14.2, 1021.9, 61, 14);
int64_t sc59 = (int64_t)((r59 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc59); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s60 = RoboticsPhysicalTempLearned_new();
double r60 = RoboticsPhysicalTempLearned_execute_step((&(s60)), 12.6, 13.6, 1021.3, 73, 12.6);
int64_t sc60 = (int64_t)((r60 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc60); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s61 = RoboticsPhysicalTempLearned_new();
double r61 = RoboticsPhysicalTempLearned_execute_step((&(s61)), 12, 12.6, 1020.7, 75, 11.9);
int64_t sc61 = (int64_t)((r61 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc61); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s62 = RoboticsPhysicalTempLearned_new();
double r62 = RoboticsPhysicalTempLearned_execute_step((&(s62)), 11.8, 12, 1019.5, 76, 15.8);
int64_t sc62 = (int64_t)((r62 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc62); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s63 = RoboticsPhysicalTempLearned_new();
double r63 = RoboticsPhysicalTempLearned_execute_step((&(s63)), 10.7, 11.8, 1018.3, 93, 16.6);
int64_t sc63 = (int64_t)((r63 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc63); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s64 = RoboticsPhysicalTempLearned_new();
double r64 = RoboticsPhysicalTempLearned_execute_step((&(s64)), 11.7, 10.7, 1016.6, 90, 18);
int64_t sc64 = (int64_t)((r64 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc64); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s65 = RoboticsPhysicalTempLearned_new();
double r65 = RoboticsPhysicalTempLearned_execute_step((&(s65)), 12.6, 11.7, 1015.1, 93, 18);
int64_t sc65 = (int64_t)((r65 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc65); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s66 = RoboticsPhysicalTempLearned_new();
double r66 = RoboticsPhysicalTempLearned_execute_step((&(s66)), 13.6, 12.6, 1014.1, 93, 18.4);
int64_t sc66 = (int64_t)((r66 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc66); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s67 = RoboticsPhysicalTempLearned_new();
double r67 = RoboticsPhysicalTempLearned_execute_step((&(s67)), 14.3, 13.6, 1013.5, 92, 15.8);
int64_t sc67 = (int64_t)((r67 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc67); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s68 = RoboticsPhysicalTempLearned_new();
double r68 = RoboticsPhysicalTempLearned_execute_step((&(s68)), 15, 14.3, 1012.9, 91, 19.1);
int64_t sc68 = (int64_t)((r68 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc68); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s69 = RoboticsPhysicalTempLearned_new();
double r69 = RoboticsPhysicalTempLearned_execute_step((&(s69)), 15.4, 15, 1012.7, 90, 17.6);
int64_t sc69 = (int64_t)((r69 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc69); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s70 = RoboticsPhysicalTempLearned_new();
double r70 = RoboticsPhysicalTempLearned_execute_step((&(s70)), 15.8, 15.4, 1012.5, 88, 19.1);
int64_t sc70 = (int64_t)((r70 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc70); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s71 = RoboticsPhysicalTempLearned_new();
double r71 = RoboticsPhysicalTempLearned_execute_step((&(s71)), 16.2, 15.8, 1011.8, 86, 20.9);
int64_t sc71 = (int64_t)((r71 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc71); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s72 = RoboticsPhysicalTempLearned_new();
double r72 = RoboticsPhysicalTempLearned_execute_step((&(s72)), 16.6, 16.2, 1011.7, 83, 20.9);
int64_t sc72 = (int64_t)((r72 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc72); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s73 = RoboticsPhysicalTempLearned_new();
double r73 = RoboticsPhysicalTempLearned_execute_step((&(s73)), 16.5, 16.6, 1011.4, 87, 22);
int64_t sc73 = (int64_t)((r73 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc73); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s74 = RoboticsPhysicalTempLearned_new();
double r74 = RoboticsPhysicalTempLearned_execute_step((&(s74)), 16.4, 16.5, 1011.2, 86, 19.4);
int64_t sc74 = (int64_t)((r74 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc74); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s75 = RoboticsPhysicalTempLearned_new();
double r75 = RoboticsPhysicalTempLearned_execute_step((&(s75)), 16.9, 16.4, 1010.6, 85, 18.4);
int64_t sc75 = (int64_t)((r75 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc75); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s76 = RoboticsPhysicalTempLearned_new();
double r76 = RoboticsPhysicalTempLearned_execute_step((&(s76)), 17.4, 16.9, 1009.7, 81, 22);
int64_t sc76 = (int64_t)((r76 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc76); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s77 = RoboticsPhysicalTempLearned_new();
double r77 = RoboticsPhysicalTempLearned_execute_step((&(s77)), 17.9, 17.4, 1008.7, 77, 23.8);
int64_t sc77 = (int64_t)((r77 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc77); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s78 = RoboticsPhysicalTempLearned_new();
double r78 = RoboticsPhysicalTempLearned_execute_step((&(s78)), 18, 17.9, 1008, 77, 24.1);
int64_t sc78 = (int64_t)((r78 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc78); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s79 = RoboticsPhysicalTempLearned_new();
double r79 = RoboticsPhysicalTempLearned_execute_step((&(s79)), 17.6, 18, 1007.7, 82, 24.1);
int64_t sc79 = (int64_t)((r79 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc79); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s80 = RoboticsPhysicalTempLearned_new();
double r80 = RoboticsPhysicalTempLearned_execute_step((&(s80)), 15.9, 17.6, 1008.9, 67, 22);
int64_t sc80 = (int64_t)((r80 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc80); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s81 = RoboticsPhysicalTempLearned_new();
double r81 = RoboticsPhysicalTempLearned_execute_step((&(s81)), 15, 15.9, 1009.7, 60, 14.8);
int64_t sc81 = (int64_t)((r81 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc81); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s82 = RoboticsPhysicalTempLearned_new();
double r82 = RoboticsPhysicalTempLearned_execute_step((&(s82)), 14.6, 15, 1010.2, 60, 14.8);
int64_t sc82 = (int64_t)((r82 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc82); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s83 = RoboticsPhysicalTempLearned_new();
double r83 = RoboticsPhysicalTempLearned_execute_step((&(s83)), 14.2, 14.6, 1010.9, 60, 15.5);
int64_t sc83 = (int64_t)((r83 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc83); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s84 = RoboticsPhysicalTempLearned_new();
double r84 = RoboticsPhysicalTempLearned_execute_step((&(s84)), 13.5, 14.2, 1011.5, 69, 14.8);
int64_t sc84 = (int64_t)((r84 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc84); _buf.ref_count = 1; _buf; }).data);
RoboticsPhysicalTempLearned s85 = RoboticsPhysicalTempLearned_new();
double r85 = RoboticsPhysicalTempLearned_execute_step((&(s85)), 12.8, 13.5, 1011.9, 67, 13.3);
int64_t sc85 = (int64_t)((r85 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc85); _buf.ref_count = 1; _buf; }).data);
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
