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
    int64_t dimension;
    int64_t modulus_bits;
    int64_t noise_budget_bits;
    int is_homomorphic;
} FheCiphertextRing;

FheCiphertextRing FheCiphertextRing_new_ckks_ring();
double FheCiphertextRing_multiply_simd_step(FheCiphertextRing*, double, double);
int FheCiphertextRing_has_sufficient_noise_budget(FheCiphertextRing*);
// module std.crypto.fhe_simd
FheCiphertextRing FheCiphertextRing_new_ckks_ring() {
return (FheCiphertextRing){.dimension = 8192LL, .modulus_bits = 128LL, .noise_budget_bits = 64LL, .is_homomorphic = 1};
}

double FheCiphertextRing_multiply_simd_step(FheCiphertextRing* self, double a_coeff, double b_coeff) {
double product = (a_coeff * b_coeff);
return product;
}

int FheCiphertextRing_has_sufficient_noise_budget(FheCiphertextRing* self) {
if (((*self).noise_budget_bits > 16LL)) {
return 1;
} else {
return 0;
}
}

