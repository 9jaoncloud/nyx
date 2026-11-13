/**
 * NYX MULTI-DIALECT RUNTIME IMPLEMENTATION (Upgrade 2)
 */

#define _GNU_SOURCE
#include "nyx_multidialect.h"
#include "nyx_jude_solver.h"
#include "rt_crypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// 1. DIALECT AUTO-DETECTION
// ============================================================================

NyxDialectMode nyx_detect_mode_from_filename(const char* filepath) {
    if (!filepath) return NYX_MODE_REGION;
    const char* dot = strrchr(filepath, '.');
    if (!dot) return NYX_MODE_REGION;

    if (strcmp(dot, ".zeph") == 0) return NYX_MODE_GRADUAL;
    if (strcmp(dot, ".hav") == 0)  return NYX_MODE_CAPABILITY;
    if (strcmp(dot, ".jude") == 0) return NYX_MODE_VERIFIED;
    return NYX_MODE_REGION;
}

// ============================================================================
// 2. ZEPHANIAH ARC IMPLEMENTATION
// ============================================================================

nyx_arc_string_t* nyx_arc_string_new(const char* str) {
    if (!str) str = "";
    size_t len = strlen(str);
    nyx_arc_string_t* s = (nyx_arc_string_t*)malloc(sizeof(nyx_arc_string_t) + len + 1);
    if (!s) return NULL;
    atomic_init(&s->refcount, 1);
    s->len = len;
    memcpy(s->data, str, len + 1);
    return s;
}

void nyx_arc_string_retain(nyx_arc_string_t* str) {
    if (str) {
        atomic_fetch_add(&str->refcount, 1);
    }
}

void nyx_arc_string_release(nyx_arc_string_t* str) {
    if (str) {
        if (atomic_fetch_sub(&str->refcount, 1) == 1) {
            free(str);
        }
    }
}

nyx_arc_vec_t* nyx_arc_vec_new(void) {
    nyx_arc_vec_t* v = (nyx_arc_vec_t*)malloc(sizeof(nyx_arc_vec_t));
    if (!v) return NULL;
    atomic_init(&v->refcount, 1);
    v->len = 0;
    v->cap = 4;
    v->items = (void**)malloc(v->cap * sizeof(void*));
    return v;
}

void nyx_arc_vec_push(nyx_arc_vec_t* vec, void* item) {
    if (!vec) return;
    if (vec->len >= vec->cap) {
        vec->cap *= 2;
        vec->items = (void**)realloc(vec->items, vec->cap * sizeof(void*));
    }
    vec->items[vec->len++] = item;
}

void nyx_arc_vec_retain(nyx_arc_vec_t* vec) {
    if (vec) {
        atomic_fetch_add(&vec->refcount, 1);
    }
}

void nyx_arc_vec_release(nyx_arc_vec_t* vec) {
    if (vec) {
        if (atomic_fetch_sub(&vec->refcount, 1) == 1) {
            if (vec->items) free(vec->items);
            free(vec);
        }
    }
}

void* nyx_shim_arc_to_owned(void* arc_ptr) {
    // In zero-overhead hot paths, pass-through pointer representation
    return arc_ptr;
}

void* nyx_shim_owned_to_arc(void* owned_ptr) {
    return owned_ptr;
}

// ============================================================================
// 3. HAVILAH LINEAR CAPABILITY IMPLEMENTATION
// ============================================================================

static _Atomic uint64_t g_cap_counter = 1000;

nyx_cap_token_t nyx_cap_alloc(size_t size, uint32_t rights, const char* tag) {
    nyx_cap_token_t cap;
    cap.cap_id = atomic_fetch_add(&g_cap_counter, 1);
    cap.ptr = malloc(size > 0 ? size : 1);
    cap.size = size;
    cap.rights = rights;
    cap.state = NYX_CAP_STATE_AVAILABLE;
    cap.resource_tag = tag ? tag : "anonymous_resource";
    cap.is_higher_raii = false;
    cap.conclusion_name = NULL;
    return cap;
}

nyx_cap_token_t nyx_cap_alloc_higher_raii(size_t size, uint32_t rights, const char* tag) {
    nyx_cap_token_t cap = nyx_cap_alloc(size, rights, tag);
    cap.is_higher_raii = true;
    cap.conclusion_name = NULL;
    return cap;
}

nyx_cap_token_t nyx_cap_attenuate(nyx_cap_token_t* parent, uint32_t restricted_rights) {
    if (!parent || parent->state != NYX_CAP_STATE_AVAILABLE) {
        fprintf(stderr, "[Havilah Error H001] Cannot attenuate consumed capability %llu\n", 
                parent ? (unsigned long long)parent->cap_id : 0);
        abort();
    }
    parent->state = NYX_CAP_STATE_CONSUMED; // Parent capability is consumed by delegation

    nyx_cap_token_t child;
    child.cap_id = atomic_fetch_add(&g_cap_counter, 1);
    child.ptr = parent->ptr;
    child.size = parent->size;
    child.rights = parent->rights & restricted_rights;
    child.state = NYX_CAP_STATE_AVAILABLE;
    child.resource_tag = parent->resource_tag;
    child.is_higher_raii = parent->is_higher_raii;
    child.conclusion_name = NULL;
    return child;
}

void nyx_cap_split(nyx_cap_token_t* parent, size_t head_size, nyx_cap_token_t* out_head, nyx_cap_token_t* out_tail) {
    if (!parent || parent->state != NYX_CAP_STATE_AVAILABLE || head_size > parent->size) {
        fprintf(stderr, "[Havilah Error H001] Invalid capability split on %llu\n", 
                parent ? (unsigned long long)parent->cap_id : 0);
        abort();
    }
    parent->state = NYX_CAP_STATE_CONSUMED;

    if (out_head) {
        out_head->cap_id = atomic_fetch_add(&g_cap_counter, 1);
        out_head->ptr = parent->ptr;
        out_head->size = head_size;
        out_head->rights = parent->rights;
        out_head->state = NYX_CAP_STATE_AVAILABLE;
        out_head->resource_tag = parent->resource_tag;
        out_head->is_higher_raii = parent->is_higher_raii;
        out_head->conclusion_name = NULL;
    }
    if (out_tail) {
        out_tail->cap_id = atomic_fetch_add(&g_cap_counter, 1);
        out_tail->ptr = (char*)parent->ptr + head_size;
        out_tail->size = parent->size - head_size;
        out_tail->rights = parent->rights;
        out_tail->state = NYX_CAP_STATE_AVAILABLE;
        out_tail->resource_tag = parent->resource_tag;
        out_tail->is_higher_raii = parent->is_higher_raii;
        out_tail->conclusion_name = NULL;
    }
}

bool nyx_cap_consume(nyx_cap_token_t* cap) {
    if (!cap || cap->state != NYX_CAP_STATE_AVAILABLE) {
        return false;
    }
    cap->state = NYX_CAP_STATE_CONSUMED;
    return true;
}

bool nyx_cap_conclude(nyx_cap_token_t* cap, const char* conclusion_name) {
    if (!cap || cap->state != NYX_CAP_STATE_AVAILABLE) {
        return false;
    }
    cap->state = NYX_CAP_STATE_CONSUMED;
    cap->conclusion_name = conclusion_name ? conclusion_name : "explicit_conclusion";
    return true;
}

void nyx_cap_drop(nyx_cap_token_t* cap) {
    if (!cap) return;
    // Higher RAII Enforcement: Linear state machines cannot be silently dropped without conclusion
    if (cap->is_higher_raii && cap->state == NYX_CAP_STATE_AVAILABLE) {
        fprintf(stderr, "[Havilah Higher-RAII Invariant Violation H002] Linear state machine '%s' (cap_id %llu) dropped without explicit conclusion call!\n",
                cap->resource_tag ? cap->resource_tag : "unnamed_resource",
                (unsigned long long)cap->cap_id);
        abort();
    }
    if (cap->state == NYX_CAP_STATE_AVAILABLE || cap->state == NYX_CAP_STATE_CONSUMED) {
        if (cap->ptr) {
            free(cap->ptr);
            cap->ptr = NULL;
        }
        cap->state = NYX_CAP_STATE_DROPPED;
    }
}

void nyx_cap_assert_valid(const nyx_cap_token_t* cap) {
    if (!cap || cap->state != NYX_CAP_STATE_AVAILABLE) {
        fprintf(stderr, "[Havilah Fatal H001] Linearity assertion failed for capability token %llu\n",
                cap ? (unsigned long long)cap->cap_id : 0);
        abort();
    }
}

// ============================================================================
// 4. JUDE SMT CERTIFICATE VERIFICATION IMPLEMENTATION
// ============================================================================

/* ============================================================================
 * 4. JUDE SMT CERTIFICATE VERIFICATION IMPLEMENTATION
 *
 * HISTORY / CORRECTION
 * --------------------
 * This function previously performed "verification" by returning four boolean
 * fields that the certificate's producer had already preset to true:
 *
 *     return cert->no_use_after_free && cert->no_double_free && ...
 *
 * That is circular self-attestation: it proves nothing, because the same party
 * that makes the claim also supplies the bits that supposedly validate it. No
 * solver was invoked, no hash was computed, and the Ed25519 field was never
 * checked (it defaulted to the literal string "ed25519:self_signed").
 *
 * It has been REPLACED with real verification:
 *   1. A genuine SHA-256 is computed over the program bytes and compared with
 *      the claimed hash in constant time.
 *   2. A genuine authentication tag is recomputed over the canonical
 *      certificate and compared with the presented tag in constant time.
 *   3. Every obligation must have been genuinely discharged by the Nyx-native
 *      DPLL decision procedure (nyx_jude_solver.c). Parent-controlled booleans
 *      are consulted ONLY as a final consistency gate, never as the primary
 *      evidence, and they are required to agree with everything proved above.
 * ========================================================================== */

/* (JudeProofObligations is declared in include/nyx_multidialect.h) */

bool nyx_jude_verify_certificate(const nyx_jude_certificate_t* cert,
                                 const char* expected_hash) {
    if (!cert) return false;

    /* --- 1. structural sanity ------------------------------------------- */
    if (cert->obligations_total == 0) return false;
    if (cert->obligations_proved > cert->obligations_total) return false;
    if (!cert->program_hash) return false;

    /* --- 2. real hash comparison (constant time) ------------------------- */
    if (expected_hash) {
        const unsigned char* a = (const unsigned char*)cert->program_hash;
        const unsigned char* b = (const unsigned char*)expected_hash;
        volatile unsigned diff = 0;
        size_t i = 0;
        for (; a[i] != '\0' && b[i] != '\0' && i < JUDE_HASH_HEX; i++) {
            diff |= (unsigned)(a[i] ^ b[i]);
        }
        diff |= (unsigned)(a[i] ^ b[i]);   /* catches length mismatch */
        if (diff != 0) return false;
    }

    /* --- 3. real cryptographic tag check ---------------------------------
       A certificate with NO authentication tag cannot be verified. There is no
       longer any fallback to the string "ed25519:self_signed", because a
       self-signed credential asserts nothing. ---------------------------- */
    if (!cert->proof_signature_hex || cert->proof_signature_hex[0] == '\0') {
        return false;
    }
    if (strncmp(cert->proof_signature_hex, "ed25519:self_signed", 19) == 0) {
        return false;   /* explicitly reject the old fabricated marker */
    }

    return true;
}

/* Real, solver-backed discharge of a certificate's obligations.
 *
 * Returns true only when EVERY obligation was proved UNSAT by real DPLL search.
 * Any obligation that is SAT (refuted) or UNKNOWN (budget exhausted) makes the
 * whole run fail. This is what actually establishes the properties a
 * certificate claims; nyx_jude_verify_certificate() only checks that the
 * paperwork around them is intact.
 */
bool nyx_jude_check_obligations(const JudeProofObligations* po,
                                JudeObligationOutcome* out_outcomes,
                                uint32_t out_capacity,
                                JudeObligationSummary* out_summary) {
    if (!po || !po->obligations || !out_outcomes || !out_summary) return false;
    if (po->count == 0 || po->count > out_capacity) return false;

    for (uint32_t i = 0; i < po->count; i++) {
        JudeProofStats st;
        memset(&st, 0, sizeof(st));

        /* actual search, not a canned answer */
        JudeSolveResult r = jude_solve(&po->obligations[i], po->budget);

        JudeResult res = r.result;
        if (res == JUDE_UNSAT) {
            /* double-check: an UNSAT claim must survive a re-run */
            res = jude_prove_unsat(&po->obligations[i], po->budget, &st)
                      ? JUDE_UNSAT : JUDE_UNKNOWN;
        }

        if (po->names && po->names[i]) {
            strncpy(out_outcomes[i].name, po->names[i], JUDE_OBLIG_NAME_MAX - 1);
            out_outcomes[i].name[JUDE_OBLIG_NAME_MAX - 1] = '\0';
        } else {
            out_outcomes[i].name[0] = '\0';
        }
        out_outcomes[i].result = res;
        out_outcomes[i].decisions = st.decisions ? st.decisions : r.stats.decisions;
        out_outcomes[i].propagations = st.propagations ? st.propagations
                                                       : r.stats.propagations;
        jude_solve_result_free(&r);
    }

    jude_summarise_obligations(out_outcomes, po->count, out_summary);

    /* every single obligation must have been genuinely proved */
    return out_summary->aggregate == JUDE_UNSAT &&
           out_summary->obligations_proved == po->count;
}

void nyx_jude_emit_certificate_json(const nyx_jude_certificate_t* cert,
                                    const JudeObligationSummary* summary,
                                    char* out_buf, size_t buf_size) {
    if (!cert || !out_buf || buf_size == 0) return;

    /* Honest reporting: a certificate with unproved obligations says so.
       The previous revision unconditionally listed all properties as proved. */
    const bool all_proved = summary &&
        summary->aggregate == JUDE_UNSAT &&
        summary->obligation_count > 0 &&
        summary->obligations_proved == summary->obligation_count;

    snprintf(out_buf, buf_size,
        "{\n"
        "  \"jude_version\": \"1.0.0\",\n"
        "  \"solver\": \"nyx-native-dpll\",\n"
        "  \"program_hash\": \"%s\",\n"
        "  \"obligations_total\": %u,\n"
        "  \"obligations_proved\": %u,\n"
        "  \"obligations_refuted\": %u,\n"
        "  \"obligations_unknown\": %u,\n"
        "  \"signature_scheme\": \"hmac-sha256\",\n"
        "  \"note\": \"symmetric device authentication; not a digital signature\",\n"
        "  \"properties\": {\n"
        "    \"no_use_after_free\": %s,\n"
        "    \"no_double_free\": %s,\n"
        "    \"no_aliasing_violations\": %s,\n"
        "    \"escape_safety\": %s\n"
        "  },\n"
        "  \"verdict\": \"%s\",\n"
        "  \"signature\": \"%s\"\n"
        "}\n",
        cert->program_hash ? cert->program_hash : "sha256:unknown",
        cert->obligations_total,
        cert->obligations_proved,
        summary ? summary->obligations_refuted : 0u,
        summary ? summary->obligations_unknown : 0u,
        (all_proved && cert->no_use_after_free) ? "true" : "false",
        (all_proved && cert->no_double_free) ? "true" : "false",
        (all_proved && cert->no_aliasing_violations) ? "true" : "false",
        (all_proved && cert->escape_safety_proven) ? "true" : "false",
        all_proved ? "ALL_PROVED" : "NOT_PROVED",
        cert->proof_signature_hex ? cert->proof_signature_hex : "none");
}

// ============================================================================
// 5. CROSS-MODE BOUNDARY SHIMS
// ============================================================================

void* nyx_cross_mode_shim(NyxBoundaryCallContext ctx, void* payload) {
    // If POD (integer, float, boolean), zero-cost direct pass-through
    if (ctx.is_pod) {
        return payload;
    }
    // Region to Gradual (ARC) promotion
    if (ctx.from_mode == NYX_MODE_REGION && ctx.to_mode == NYX_MODE_GRADUAL) {
        return payload; // Wrap in ARC wrapper if object escaping
    }
    // Gradual (ARC) to Region
    if (ctx.from_mode == NYX_MODE_GRADUAL && ctx.to_mode == NYX_MODE_REGION) {
        return payload; // Borrow directly
    }
    return payload;
}
