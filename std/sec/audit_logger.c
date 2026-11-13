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
    rt_string_t function_name;
    int64_t* original_sha256;
    int64_t* transformed_sha256;
    int64_t applied_transforms_mask;
    int64_t instruction_count_delta;
    double timing_delta_percentage;
} AuditRecord;

typedef struct {
    int64_t build_seed;
    rt_string_t compiler_version;
    rt_string_t target_triple;
    int64_t* overall_binary_hash;
    int64_t timestamp_utc;
    int is_deterministic;
    rt_string_t asil_level;
} AuditManifest;

typedef struct {
    int64_t seed;
    rt_string_t target_arch;
    rt_string_t asil_rating;
} AuditLogger;

// import std::crypto
// import std::time
AuditLogger AuditLogger_new(int64_t seed, rt_string_t arch, rt_string_t asil) {
return (AuditLogger){.seed = seed, .target_arch = arch, .asil_rating = asil};
}

AuditRecord AuditLogger_log_transformation(AuditLogger* self, rt_string_t func_name, NyxVec* raw_code, NyxVec* hardened_code, int64_t transform_flags) {
crypto orig_hash = crypto_sha256(raw_code);
crypto hard_hash = crypto_sha256(hardened_code);
int64_t delta_instr = ((int64_t)(hardened_code->len) - (int64_t)(raw_code->len));
double timing_delta = ({ double __if_res_1; if ((raw_code->len > 0LL)) {
__if_res_1 = (((double)(delta_instr) / (double)(raw_code->len)) * 100);
} else {
__if_res_1 = 0;
}
__if_res_1; });
return (AuditRecord){.function_name = func_name, .original_sha256 = orig_hash, .transformed_sha256 = hard_hash, .applied_transforms_mask = transform_flags, .instruction_count_delta = delta_instr, .timing_delta_percentage = timing_delta};
}

AuditManifest AuditLogger_create_manifest(AuditLogger* self, NyxVec* final_binary, rt_string_t compiler_ver) {
crypto bin_hash = crypto_sha256(final_binary);
return (AuditManifest){.build_seed = (*self).seed, .compiler_version = compiler_ver, .target_triple = (*self).target_arch, .overall_binary_hash = bin_hash, .timestamp_utc = 1789128000LL, .is_deterministic = 1, .asil_level = (*self).asil_rating};
}

int AuditLogger_verify_delta_stability(AuditLogger* self, AuditRecord* baseline_record, AuditRecord* patched_record, int is_function_expected_to_change) {
({ if ((!is_function_expected_to_change)) {
int64_t diff = 0LL;
int64_t i = 0LL;
while ((i < 32LL)) {
diff = (diff | ((*(int64_t*)rt_vec_get((*baseline_record).transformed_sha256, i)) ^ (*(int64_t*)rt_vec_get((*patched_record).transformed_sha256, i))));
i = (i + 1LL);
}
;
return (diff == 0LL);
}
});
return 1;
}

