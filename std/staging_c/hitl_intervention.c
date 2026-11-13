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
    rt_string_t tool_id;
    rt_string_t epistemic_gap_description;
    rt_string_t smt_failure_log;
    double fitness_score;
    int64_t timestamp_us;
    rt_string_t severity;
} DiagnosticPacket;

typedef struct {
    int is_awaiting_human;
    int64_t intervention_count;
    int64_t resolved_count;
    DiagnosticPacket current_diagnostic;
    int operator_override_signal;
} InterventionState;

InterventionState InterventionState_new();
int InterventionState_check_intervention_needed(InterventionState*, double, int);
void InterventionState_trigger_intervention(InterventionState*, rt_string_t, rt_string_t, rt_string_t, double);
void InterventionState_resolve_intervention(InterventionState*);
rt_string_t InterventionState_get_status_string(InterventionState*);
rt_string_t InterventionState_serialize_diagnostic(InterventionState*);
void nyx_main();
// module sophia.phase2.hitl_intervention
InterventionState InterventionState_new() {
return (InterventionState){.is_awaiting_human = 0, .intervention_count = 0LL, .resolved_count = 0LL, .current_diagnostic = (DiagnosticPacket){.tool_id = rt_string_from("none"), .epistemic_gap_description = rt_string_from("none"), .smt_failure_log = rt_string_from("none"), .fitness_score = 1, .timestamp_us = 0LL, .severity = rt_string_from("none")}, .operator_override_signal = 0};
}

int InterventionState_check_intervention_needed(InterventionState* self, double fitness, int has_gap) {
({ if ((fitness < 0.7)) {
return 1;
}
});
({ if ((has_gap == 1)) {
return 1;
}
});
return 0;
}

void InterventionState_trigger_intervention(InterventionState* self, rt_string_t tool_id, rt_string_t gap_desc, rt_string_t smt_log, double fitness) {
(self->is_awaiting_human = 1);
(self->intervention_count = ((*self).intervention_count + 1LL));
rt_string_t severity = rt_string_from("LOW");
({ rt_string_t __if_res_3; if ((fitness < 0.3)) {
__if_res_3 = (severity = rt_string_from("CRITICAL"));
} else {
if ((fitness < 0.5)) {
__if_res_3 = (severity = rt_string_from("HIGH"));
} else {
({ if ((fitness < 0.7)) {
severity = rt_string_from("MEDIUM");
}
});
}
}
__if_res_3; });
(self->current_diagnostic = (DiagnosticPacket){.tool_id = tool_id, .epistemic_gap_description = gap_desc, .smt_failure_log = smt_log, .fitness_score = fitness, .timestamp_us = 0LL, .severity = severity});
}

void InterventionState_resolve_intervention(InterventionState* self) {
(self->is_awaiting_human = 0);
(self->resolved_count = ((*self).resolved_count + 1LL));
(self->operator_override_signal = 0);
}

rt_string_t InterventionState_get_status_string(InterventionState* self) {
({ if (((*self).is_awaiting_human == 1)) {
return rt_string_from("AWAITING_HUMAN_INTERVENTION");
}
});
return rt_string_from("AUTONOMOUS_CYCLE_ACTIVE");
}

rt_string_t InterventionState_serialize_diagnostic(InterventionState* self) {
rt_string_t s = rt_string_from("{\"tool_id\":\"");
s = rt_string_concat(s, ((*self).current_diagnostic).tool_id);
s = rt_string_concat(s, rt_string_from("\",\"gap\":\""));
s = rt_string_concat(s, ((*self).current_diagnostic).epistemic_gap_description);
s = rt_string_concat(s, rt_string_from("\",\"smt_log\":\""));
s = rt_string_concat(s, ((*self).current_diagnostic).smt_failure_log);
s = rt_string_concat(s, rt_string_from("\",\"fitness\":\""));
s = rt_string_concat(s, ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", ((*self).current_diagnostic).fitness_score); _buf.ref_count = 1; _buf; }));
s = rt_string_concat(s, rt_string_from("\",\"severity\":\""));
s = rt_string_concat(s, ((*self).current_diagnostic).severity);
s = rt_string_concat(s, rt_string_from("\",\"intervention_count\":\""));
s = rt_string_concat(s, ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)(*self).intervention_count); _buf.ref_count = 1; _buf; }));
s = rt_string_concat(s, rt_string_from("\"}"));
return s;
}

void nyx_main() {
Arena* __arena = arena_create();
printf("%s\n", rt_string_from("=== SOPHIA PHASE II: HITL INTERVENTION ===").data);
InterventionState state = InterventionState_new();
printf("Initial status: %s\n", InterventionState_get_status_string(&(state)).data);
int need1 = InterventionState_check_intervention_needed(&(state), 0.85, 0);
printf("Fitness=0.85, no gap: intervention needed = %p\n", need1);
int need2 = InterventionState_check_intervention_needed(&(state), 0.65, 0);
printf("Fitness=0.65, no gap: intervention needed = %p\n", need2);
int need3 = InterventionState_check_intervention_needed(&(state), 0.9, 1);
printf("Fitness=0.90, has gap: intervention needed = %p\n", need3);
assert_true((need1 == 0));
assert_true((need2 == 1));
assert_true((need3 == 1));
InterventionState state2 = InterventionState_new();
InterventionState_trigger_intervention(&(state2), rt_string_from("tool_042"), rt_string_from("Candidate tool has no registered epistemic profile"), rt_string_from("SMT proof failed: unbounded loop detected in branch 3"), 0.45);
printf("After trigger: %s\n", InterventionState_get_status_string(&(state2)).data);
printf("Diagnostic: %s\n", InterventionState_serialize_diagnostic(&(state2)).data);
assert_true((state2.is_awaiting_human == 1));
assert_true((state2.intervention_count == 1LL));
InterventionState_resolve_intervention(&(state2));
printf("After resolve: %s\n", InterventionState_get_status_string(&(state2)).data);
assert_true((state2.is_awaiting_human == 0));
assert_true((state2.resolved_count == 1LL));
printf("%s\n", rt_string_from("=== HITL INTERVENTION: ALL TESTS PASSED ===").data);
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
