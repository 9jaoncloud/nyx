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
    int64_t id;
    rt_string_t entity_type;
    rt_string_t value;
    double threat_score;
} ThreatEntity;

double ThreatEntity_compute_threat(ThreatEntity*);
CalcOp decide_action(double);
void nyx_main();
// module aegis.core
double ThreatEntity_compute_threat(ThreatEntity* self) {
double base = (*self).threat_score;
int64_t length_factor = ((double)((*self).value.length) * 0.01);
return (base + length_factor);
}

CalcOp decide_action(double threat) {
if ((threat < 0.3)) {
return (CalcOp){ .tag = TAG_CalcOp_None };
} else if ((threat < 0.7)) {
return (CalcOp){ .tag = TAG_CalcOp_Add };
} else {
return (CalcOp){ .tag = TAG_CalcOp_Multiply };
}
}

void nyx_main() {
printf("%s\n", rt_string_from("==================================================").data);
printf("%s\n", rt_string_from("🛡️  AEGIS SENTINEL - CORE ENGINE").data);
printf("%s\n", rt_string_from("==================================================").data);
ThreatEntity entity = (ThreatEntity){.id = 1LL, .entity_type = rt_string_from("intruder"), .value = rt_string_from("unknown_person_at_door"), .threat_score = 0.6};
double threat = ThreatEntity_compute_threat(&(entity));
CalcOp action = decide_action(threat);
printf("%s%s%lld\n", rt_string_from("Entity: {} (id: {})").data, entity.value.data, (long long)(entity.id));
printf("%s%f\n", rt_string_from("Threat score: {}").data, threat);
printf("%s%p\n", rt_string_from("Action: {:?}").data, action);
printf("%s\n", rt_string_from("==================================================").data);
printf("%s\n", rt_string_from("✅ Aegis Core Engine operational!").data);
printf("%s\n", rt_string_from("==================================================").data);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
