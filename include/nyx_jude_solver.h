/**
 * JUDE NATIVE DECISION PROCEDURE (Nyx-language-owned, zero external deps)
 * ========================================================================
 * Jude (.jude) is the Nyx dialect responsible for formal verification.
 *
 * This module implements a REAL DPLL satisfiability solver in Nyx-owned C.
 * No external solver (Z3/CVC5/Cadical) is linked. No results are hardcoded.
 *
 * Honesty contract enforced throughout this file:
 *   - Every result is produced by actual search over the clause database.
 *   - UNKNOWN is returned when the search budget is exhausted. It is NEVER
 *     silently coerced into "proved".
 *   - OBLIGATION_PROVEN requires an UNSAT result from real search.
 *   - Certificate integrity uses the Nyx runtime's real SHA-256/HMAC-SHA-256
 *     (runtime/rt_crypto.c), which is validated against NIST vectors.
 */

#ifndef NYX_JUDE_SOLVER_H
#define NYX_JUDE_SOLVER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ==========================================================================
 * 1. RESULT TRUTH STATES
 * ========================================================================== */

typedef enum {
    JUDE_UNSAT  = 0,   /* property PROVED   : negation unsatisfiable */
    JUDE_SAT    = 1,   /* property REFUTED  : counterexample exists  */
    JUDE_UNKNOWN = 2   /* budget exhausted  : NOT proved, NOT refuted */
} JudeResult;

const char* jude_result_name(JudeResult r);

/* ==========================================================================
 * 2. CNF KNOWLEDGE BASE
 * ========================================================================== */

/* A literal is a non-zero int: positive = variable v, negative = negation.
   Variables are 1-indexed; literal 0 is the terminator sentinel. */
typedef struct {
    int32_t* literals;      /* all clause literals, back-to-back        */
    int32_t* clause_start;  /* offsets into literals, one per clause     */
    int32_t* clause_len;    /* length of each clause                     */
    int32_t  clause_count;
    int32_t  reserved;      /* capacity of clause arrays                 */
    int32_t  var_count;
} JudeCnf;

void      jude_cnf_init(JudeCnf* cnf, int32_t var_count);
void      jude_cnf_free(JudeCnf* cnf);
bool      jude_cnf_add_clause(JudeCnf* cnf, const int32_t* lits, int32_t n);

/* ==========================================================================
 * 3. SOLVER WITH REAL SEARCH ACCOUNTING
 * ========================================================================== */

typedef struct {
    uint64_t decisions;      /* actual branching decisions taken   */
    uint64_t propagations;   /* actual unit propagations performed */
    uint64_t conflicts;      /* actual conflicts encountered       */
    uint64_t budget;         /* max decisions before giving up     */
} JudeProofStats;

typedef struct {
    JudeResult    result;
    JudeProofStats stats;
    int32_t*      model;       /* var_count+1 values, only if SAT   */
    int32_t       model_size;
} JudeSolveResult;

/* Core entry point: real DPLL with unit propagation. */
JudeSolveResult jude_solve(const JudeCnf* cnf, uint64_t budget);

/* Convenience: prove that a formula is unsatisfiable. Returns true ONLY when
   real search returned UNSAT. Anything else (SAT/UNKNOWN) returns false. */
bool jude_prove_unsat(const JudeCnf* cnf, uint64_t budget, JudeProofStats* out_stats);

void jude_solve_result_free(JudeSolveResult* res);

/* ==========================================================================
 * 4. REAL CERTIFICATES (cryptographic, not self-attested)
 * ========================================================================== */

#define JUDE_HASH_HEX  65   /* 64 hex chars + NUL               */
#define JUDE_TAG_HEX   65   /* 64 hex chars + NUL (HMAC-SHA256) */
#define JUDE_OBLIG_NAME_MAX 64

typedef struct {
    char name[JUDE_OBLIG_NAME_MAX];
    JudeResult result;       /* must be JUDE_UNSAT to count as proved */
    uint64_t decisions;
    uint64_t propagations;
} JudeObligationOutcome;

typedef struct {
    uint32_t obligation_count;
    uint32_t obligations_proved;   /* counted from real results only */
    uint32_t obligations_refuted;
    uint32_t obligations_unknown;
    JudeResult aggregate;          /* worst case across all obligations */
} JudeObligationSummary;

/* Summarise outcomes honestly. Any non-proved obligation downgrades the
   aggregate result. This replaces the previous behaviour of declaring the
   whole certificate valid because every bool was preset to true. */
void jude_summarise_obligations(const JudeObligationOutcome* outcomes,
                                uint32_t count,
                                JudeObligationSummary* out);

/* Canonical serialisation used for hashing. Deterministic byte-for-byte. */
void jude_certificate_canonical(const JudeObligationSummary* summary,
                                const char* solver_version,
                                const uint8_t* program_hash,   /* 32 bytes */
                                char* out_buf, size_t buf_size);

/* Real HMAC-SHA256 authentication tag over the canonical certificate body.
   NOTE HONESTLY: this is symmetric authentication (integrity + device
   attestation), NOT an unforgeable digital signature. Until Nyx ships a real
   Ed25519 implementation we refuse to print "Ed25519". */
bool jude_certificate_sign(const char* canonical_body,
                           const uint8_t* key, size_t key_len,
                           char* out_tag_hex);   /* JUDE_TAG_HEX */

/* Constant-time compare of two authentication tags. */
bool jude_tag_equals(const char* a, const char* b);

/* Real end-to-end verification: recompute the tag and compare in constant
   time, and require every obligation to have genuinely proved. Returns false
   on any tampering or any unproved obligation. */
bool jude_certificate_verify(const char* canonical_body,
                             const char* presented_tag_hex,
                             const uint8_t* key, size_t key_len,
                             const JudeObligationSummary* summary);

#ifdef __cplusplus
}
#endif

#endif /* NYX_JUDE_SOLVER_H */
