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
    double primary_metric;
    int is_converged;
} TestSolver;

TestSolver TestSolver_new();
double TestSolver_evaluate_primary_objective(TestSolver*, double);
int TestSolver_compute_safety_invariant(TestSolver*, double);
double TestSolver_execute_step(TestSolver*, double);
void nyx_main();
extern double sin(double);
extern double fabs(double);
TestSolver TestSolver_new() {
return (TestSolver){.primary_metric = 1, .is_converged = 1};
}

double TestSolver_evaluate_primary_objective(TestSolver* self, double input_signal) {
double dynamic_gain = (1 + ({
sin((input_signal * 0.05));
}));
double normalized = (input_signal * dynamic_gain);
if ((normalized > 100000)) {
return 100000;
} else if ((normalized < (-100000))) {
return (-100000);
} else {
return normalized;
}
}

int TestSolver_compute_safety_invariant(TestSolver* self, double current_state) {
double err = ({
fabs(current_state);
});
return ((err <= 100000) && (*self).is_converged);
}

double TestSolver_execute_step(TestSolver* self, double step_delta) {
double response = TestSolver_evaluate_primary_objective(self, step_delta);
(self->is_converged = TestSolver_compute_safety_invariant(self, response));
return response;
}

void nyx_main() {
Arena* __arena = arena_create();
TestSolver solver = TestSolver_new();
double res = TestSolver_execute_step((&(solver)), 1);
int64_t res_i = (int64_t)(res);
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)res_i); _buf.ref_count = 1; _buf; }).data);
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
