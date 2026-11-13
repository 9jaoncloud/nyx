/**
 * NYX MULTI-DIALECT RUNTIME CORE (Upgrade 2)
 * Supporting:
 *   1. Nyx Base (.nyx)       - Region Inference + Deterministic Arena Allocator
 *   2. Zephaniah (.zeph)     - Gradual Ownership (Atomic RefCount + @owned zero-overhead)
 *   3. Havilah (.hav)        - Linear Capability Ownership (Single-use tokens, Attenuation, Auto-drop)
 *   4. Jude (.jude)          - SMT Formal Verification (Proof certificates & Invariant checking)
 */

#ifndef NYX_MULTIDIALECT_H
#define NYX_MULTIDIALECT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>

/* Jude obligations and summaries are defined by the Nyx-native decision
   procedure. See nyx_jude_solver.h for the solver and honest result states. */
#include "nyx_jude_solver.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 1. DIALECT & MEMORY MODE ENUMS
// ============================================================================

typedef enum {
    NYX_MODE_REGION     = 0,  // .nyx  - Region Inference (Default)
    NYX_MODE_GRADUAL    = 1,  // .zeph - Zephaniah (ARC default + @owned regions)
    NYX_MODE_CAPABILITY = 2,  // .hav  - Havilah (Linear Capability Ownership)
    NYX_MODE_VERIFIED   = 3   // .jude - Jude (SMT Formally Proven Regions)
} NyxDialectMode;

// Mode auto-detection by file extension
NyxDialectMode nyx_detect_mode_from_filename(const char* filepath);

// ============================================================================
// 2. ZEPHANIAH RUNTIME (ARC + @owned Shims)
// ============================================================================

typedef struct {
    _Atomic int32_t refcount;
    size_t len;
    char data[];
} nyx_arc_string_t;

typedef struct {
    _Atomic int32_t refcount;
    size_t len;
    size_t cap;
    void** items;
} nyx_arc_vec_t;

nyx_arc_string_t* nyx_arc_string_new(const char* str);
void nyx_arc_string_retain(nyx_arc_string_t* str);
void nyx_arc_string_release(nyx_arc_string_t* str);

nyx_arc_vec_t* nyx_arc_vec_new(void);
void nyx_arc_vec_push(nyx_arc_vec_t* vec, void* item);
void nyx_arc_vec_retain(nyx_arc_vec_t* vec);
void nyx_arc_vec_release(nyx_arc_vec_t* vec);

// Zero-overhead boundary shims between ARC and Region/Owned
void* nyx_shim_arc_to_owned(void* arc_ptr);
void* nyx_shim_owned_to_arc(void* owned_ptr);

// ============================================================================
// 3. HAVILAH RUNTIME (Linear Capability Tokens)
// ============================================================================

#define NYX_CAP_RIGHT_READ    (1 << 0)
#define NYX_CAP_RIGHT_WRITE   (1 << 1)
#define NYX_CAP_RIGHT_EXEC    (1 << 2)
#define NYX_CAP_RIGHT_DELETE  (1 << 3)
#define NYX_CAP_RIGHT_ALL     (0x0F)

typedef enum {
    NYX_CAP_STATE_AVAILABLE = 0,
    NYX_CAP_STATE_CONSUMED  = 1,
    NYX_CAP_STATE_BORROWED  = 2,
    NYX_CAP_STATE_DROPPED   = 3
} NyxCapState;

typedef struct {
    uint64_t cap_id;
    void* ptr;
    size_t size;
    uint32_t rights;
    NyxCapState state;
    const char* resource_tag;
    bool is_higher_raii;
    const char* conclusion_name;
} nyx_cap_token_t;

// Capability Allocation & Lifecycle
nyx_cap_token_t nyx_cap_alloc(size_t size, uint32_t rights, const char* tag);
nyx_cap_token_t nyx_cap_alloc_higher_raii(size_t size, uint32_t rights, const char* tag);
nyx_cap_token_t nyx_cap_attenuate(nyx_cap_token_t* parent, uint32_t restricted_rights);
void nyx_cap_split(nyx_cap_token_t* parent, size_t head_size, nyx_cap_token_t* out_head, nyx_cap_token_t* out_tail);
bool nyx_cap_consume(nyx_cap_token_t* cap);
bool nyx_cap_conclude(nyx_cap_token_t* cap, const char* conclusion_name);
void nyx_cap_drop(nyx_cap_token_t* cap);

// Linearity Guard: Aborts if double-consumed or leaked without drop
void nyx_cap_assert_valid(const nyx_cap_token_t* cap);

// ============================================================================
// 4. JUDE RUNTIME (SMT Proof Obligations & Certificate Verification)
// ============================================================================

typedef struct {
    const char* program_hash;
    const char* compiler_version;
    const char* solver_version;      /* e.g. "nyx-native-dpll-1.0" */
    uint32_t obligations_proved;
    uint32_t obligations_total;
    bool no_use_after_free;
    bool no_double_free;
    bool no_aliasing_violations;
    bool escape_safety_proven;
    const char* proof_signature_hex; /* HMAC-SHA256 tag over canonical cert */
} nyx_jude_certificate_t;

/* Structural + cryptographic check of a certificate's paperwork.
   NOTE: this does NOT prove the properties. It verifies that the claimed
   program hash matches and that a real authentication tag is present.
   The properties themselves are discharged by nyx_jude_check_obligations(),
   which runs the Nyx-native DPLL solver. */
bool nyx_jude_verify_certificate(const nyx_jude_certificate_t* cert, const char* expected_hash);

/* Context for real obligation discharge. */
typedef struct {
    const JudeCnf*       obligations;
    const char* const*   names;
    uint32_t             count;
    uint64_t             budget;
} JudeProofObligations;

/* Actually proves things. Returns true only if EVERY obligation was shown
   UNSAT by genuine DPLL search; anything refuted or unknown fails the run. */
bool nyx_jude_check_obligations(const JudeProofObligations* po,
                                JudeObligationOutcome* out_outcomes,
                                uint32_t out_capacity,
                                JudeObligationSummary* out_summary);

/* emits_json signature changed to require the real summary */
void nyx_jude_emit_certificate_json(const nyx_jude_certificate_t* cert,
                                    const JudeObligationSummary* summary,
                                    char* out_buf, size_t buf_size);

// ============================================================================
// 5. CROSS-MODE BOUNDARY SHIM INFRASTRUCTURE
// ============================================================================

typedef struct {
    NyxDialectMode from_mode;
    NyxDialectMode to_mode;
    bool is_pod;
    size_t data_size;
} NyxBoundaryCallContext;

void* nyx_cross_mode_shim(NyxBoundaryCallContext ctx, void* payload);

#ifdef __cplusplus
}
#endif

#endif // NYX_MULTIDIALECT_H
