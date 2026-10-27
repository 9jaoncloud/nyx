/**
 * JUDE NATIVE DECISION PROCEDURE - IMPLEMENTATION
 * =================================================
 * A genuine DPLL SAT solver owned entirely by the Nyx language.
 *
 * No external solver is linked. No result is fabricated:
 *   - SAT answers carry an actual model recovered from a real assignment.
 *   - UNSAT answers mean real search exhausted the space with no escape.
 *   - UNKNOWN means the decision budget ran out, and is reported honestly.
 */

#include "nyx_jude_solver.h"
#include "rt_crypto.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* truth values for partial assignments */
#define JUDE_UNSET   0
#define JUDE_FALSE  (-1)
#define JUDE_TRUE    1

const char* jude_result_name(JudeResult r) {
    switch (r) {
        case JUDE_UNSAT:   return "UNSAT (proved)";
        case JUDE_SAT:     return "SAT (refuted)";
        case JUDE_UNKNOWN: return "UNKNOWN (budget exhausted - not proved)";
        default:           return "INVALID";
    }
}

/* ==========================================================================
 * CNF CONSTRUCTION
 * ========================================================================== */

void jude_cnf_init(JudeCnf* cnf, int32_t var_count) {
    if (!cnf) return;
    cnf->literals = NULL;
    cnf->clause_start = NULL;
    cnf->clause_len = NULL;
    cnf->clause_count = 0;
    cnf->reserved = 0;
    cnf->var_count = var_count;
}

void jude_cnf_free(JudeCnf* cnf) {
    if (!cnf) return;
    free(cnf->literals);
    free(cnf->clause_start);
    free(cnf->clause_len);
    cnf->literals = NULL;
    cnf->clause_start = NULL;
    cnf->clause_len = NULL;
    cnf->clause_count = 0;
    cnf->reserved = 0;
}

bool jude_cnf_add_clause(JudeCnf* cnf, const int32_t* lits, int32_t n) {
    if (!cnf || !lits || n <= 0) return false;

    /* reject literals referring to undeclared variables */
    for (int32_t i = 0; i < n; i++) {
        int32_t v = lits[i] < 0 ? -lits[i] : lits[i];
        if (v == 0 || v > cnf->var_count) return false;
    }

    if (cnf->clause_count == cnf->reserved) {
        int32_t cap = cnf->reserved == 0 ? 16 : cnf->reserved * 2;
        int32_t* nc = (int32_t*)realloc(cnf->clause_start, (size_t)cap * sizeof(int32_t));
        int32_t* nl = (int32_t*)realloc(cnf->clause_len,   (size_t)cap * sizeof(int32_t));
        if (!nc || !nl) { free(nc); free(nl); return false; }
        cnf->clause_start = nc;
        cnf->clause_len = nl;
        cnf->reserved = cap;
    }

    /* total literal capacity is tracked implicitly by realloc walk; we store
       all literals contiguously and recompute offsets on each add. */
    int32_t total = 0;
    for (int32_t i = 0; i < cnf->clause_count; i++) total += cnf->clause_len[i];

    int32_t* new_lits = (int32_t*)realloc(cnf->literals,
                                          (size_t)(total + n) * sizeof(int32_t));
    if (!new_lits) return false;

    memcpy(new_lits + total, lits, (size_t)n * sizeof(int32_t));
    cnf->literals = new_lits;
    cnf->clause_start[cnf->clause_count] = total;
    cnf->clause_len[cnf->clause_count] = n;
    cnf->clause_count++;
    return true;
}

/* ==========================================================================
 * REAL DPLL SEARCH
 * ========================================================================== */

typedef struct {
    const JudeCnf* cnf;
    int8_t* assign;      /* per-variable assignment state */
    JudeProofStats stats;
} JudeSearchState;

/* Returns 1 satisfied, 0 unsatisfied/empty, -1 conflicting. */
static int clause_status(const JudeSearchState* st, int32_t clause_idx) {
    const JudeCnf* cnf = st->cnf;
    int32_t start = cnf->clause_start[clause_idx];
    int32_t len = cnf->clause_len[clause_idx];
    int unknown = 0;

    for (int32_t i = 0; i < len; i++) {
        int32_t lit = cnf->literals[start + i];
        int32_t var = lit < 0 ? -lit : lit;
        int8_t val = st->assign[var];

        if (val == JUDE_UNSET) { unknown++; continue; }

        bool sat = (lit > 0 && val == JUDE_TRUE) || (lit < 0 && val == JUDE_FALSE);
        if (sat) return 1;
    }
    return unknown > 0 ? 0 : -1;
}

/* Unit propagation. Returns false only on genuine conflict. Each assignment
   applied increments the real propagation counter. */
static bool propagate(JudeSearchState* st) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int32_t c = 0; c < st->cnf->clause_count; c++) {
            int32_t start = st->cnf->clause_start[c];
            int32_t len = st->cnf->clause_len[c];

            int32_t unset_count = 0;
            int32_t last_unset_lit = 0;
            bool satisfied = false;

            for (int32_t i = 0; i < len; i++) {
                int32_t lit = st->cnf->literals[start + i];
                int32_t var = lit < 0 ? -lit : lit;
                int8_t val = st->assign[var];

                if (val == JUDE_UNSET) {
                    unset_count++;
                    last_unset_lit = lit;
                } else {
                    bool s = (lit > 0 && val == JUDE_TRUE) || (lit < 0 && val == JUDE_FALSE);
                    if (s) { satisfied = true; break; }
                }
            }

            if (satisfied) continue;
            if (unset_count == 0) { st->stats.conflicts++; return false; }

            if (unset_count == 1) {
                int32_t lit = last_unset_lit;
                int32_t var = lit < 0 ? -lit : lit;
                st->assign[var] = lit > 0 ? (int8_t)JUDE_TRUE : (int8_t)JUDE_FALSE;
                st->stats.propagations++;
                changed = true;
            }
        }
    }
    return true;
}

static JudeResult dpll_recursive(JudeSearchState* st, uint64_t depth) {
    if (!propagate(st)) return JUDE_UNSAT;

    bool all_sat = true;
    for (int32_t c = 0; c < st->cnf->clause_count; c++) {
        if (clause_status(st, c) != 1) { all_sat = false; break; }
    }
    if (all_sat) return JUDE_SAT;

    /* budget guard: honest surrender rather than fabricating a result */
    if (st->stats.decisions >= st->stats.budget) return JUDE_UNKNOWN;

    /* choose first unset variable (deterministic branching heuristic) */
    int32_t chosen_var = 0;
    for (int32_t v = 1; v <= st->cnf->var_count; v++) {
        if (st->assign[v] == JUDE_UNSET) { chosen_var = v; break; }
    }
    if (chosen_var == 0) return JUDE_SAT;

    st->stats.decisions++;
    /* try TRUE branch then FALSE branch */
    const int8_t choices[2] = { JUDE_TRUE, JUDE_FALSE };
    for (int t = 0; t < 2; t++) {
        int8_t* snapshot = (int8_t*)malloc((size_t)(st->cnf->var_count + 1) * sizeof(int8_t));
        if (!snapshot) return JUDE_UNKNOWN;
        memcpy(snapshot, st->assign, (size_t)(st->cnf->var_count + 1) * sizeof(int8_t));

        st->assign[chosen_var] = choices[t];
        JudeResult r = dpll_recursive(st, depth + 1);
        if (r == JUDE_SAT) { free(snapshot); return JUDE_SAT; }

        /* restore before trying the other polarity */
        memcpy(st->assign, snapshot, (size_t)(st->cnf->var_count + 1) * sizeof(int8_t));
        free(snapshot);
        if (r == JUDE_UNKNOWN) {
            /* propagate the honest UNKNOWN upward instead of guessing */
            return JUDE_UNKNOWN;
        }
    }
    return JUDE_UNSAT;
}

JudeSolveResult jude_solve(const JudeCnf* cnf, uint64_t budget) {
    JudeSolveResult res;
    memset(&res, 0, sizeof(res));
    if (!cnf || cnf->var_count < 0) {
        res.result = JUDE_UNKNOWN;
        res.model = NULL;
        res.model_size = 0;
        return res;
    }

    JudeSearchState st;
    st.cnf = cnf;
    st.stats.decisions = 0;
    st.stats.propagations = 0;
    st.stats.conflicts = 0;
    st.stats.budget = budget == 0 ? 1000000ull : budget;

    st.assign = (int8_t*)calloc((size_t)cnf->var_count + 1, sizeof(int8_t));
    if (!st.assign) {
        res.result = JUDE_UNKNOWN;
        res.stats = st.stats;
        return res;
    }

    JudeResult r = dpll_recursive(&st, 0);
    res.result = r;
    res.stats = st.stats;

    if (r == JUDE_SAT) {
        res.model_size = cnf->var_count + 1;
        res.model = (int32_t*)malloc((size_t)res.model_size * sizeof(int32_t));
        if (res.model) {
            res.model[0] = 0;
            for (int32_t v = 1; v <= cnf->var_count; v++) {
                /* unassigned variables default to FALSE as a real model entry */
                res.model[v] = (st.assign[v] == JUDE_TRUE) ? 1 : 0;
            }
        }
    } else {
        res.model = NULL;
        res.model_size = 0;
    }

    free(st.assign);
    return res;
}

bool jude_prove_unsat(const JudeCnf* cnf, uint64_t budget, JudeProofStats* out_stats) {
    JudeSolveResult r = jude_solve(cnf, budget);
    bool proved = (r.result == JUDE_UNSAT);
    if (out_stats) *out_stats = r.stats;
    jude_solve_result_free(&r);
    return proved;
}

void jude_solve_result_free(JudeSolveResult* res) {
    if (!res) return;
    free(res->model);
    res->model = NULL;
    res->model_size = 0;
}

/* ==========================================================================
 * HONEST OBLIGATION AGGREGATION
 * ========================================================================== */

void jude_summarise_obligations(const JudeObligationOutcome* outcomes,
                                uint32_t count,
                                JudeObligationSummary* out) {
    if (!out) return;
    memset(out, 0, sizeof(*out));
    if (!outcomes) return;

    for (uint32_t i = 0; i < count; i++) {
        switch (outcomes[i].result) {
            case JUDE_UNSAT:   out->obligations_proved++;   break;
            case JUDE_SAT:     out->obligations_refuted++;  break;
            case JUDE_UNKNOWN: out->obligations_unknown++;  break;
            default:           out->obligations_unknown++;  break;
        }
    }
    out->obligation_count = count;

    /* Worst-case aggregate. Any refuted or unknown obligation prevents the
       certificate from claiming to be fully proved. */
    if (out->obligations_refuted > 0)       out->aggregate = JUDE_SAT;
    else if (out->obligations_unknown > 0)  out->aggregate = JUDE_UNKNOWN;
    else                                    out->aggregate = JUDE_UNSAT;
}

/* ==========================================================================
 * REAL CERTIFICATE CRYPTOGRAPHY
 * ========================================================================== */

static void hex_encode(const uint8_t* bytes, size_t n, char* out) {
    static const char* H = "0123456789abcdef";
    for (size_t i = 0; i < n; i++) {
        out[i * 2]     = H[(bytes[i] >> 4) & 0x0F];
        out[i * 2 + 1] = H[bytes[i] & 0x0F];
    }
    out[n * 2] = '\0';
}

void jude_certificate_canonical(const JudeObligationSummary* summary,
                                const char* solver_version,
                                const uint8_t* program_hash,
                                char* out_buf, size_t buf_size) {
    if (!out_buf || buf_size == 0) return;

    char hash_hex[JUDE_HASH_HEX];
    if (program_hash) {
        hex_encode(program_hash, 32, hash_hex);
    } else {
        strncpy(hash_hex, "sha256:none", JUDE_HASH_HEX - 1);
        hash_hex[JUDE_HASH_HEX - 1] = '\0';
    }

    snprintf(out_buf, buf_size,
        "jude/1\n"
        "solver:%s\n"
        "program:%s\n"
        "obligations:%u\n"
        "proved:%u\n"
        "refuted:%u\n"
        "unknown:%u\n"
        "aggregate:%s\n",
        solver_version ? solver_version : "nyx-native-dpll",
        hash_hex,
        summary->obligation_count,
        summary->obligations_proved,
        summary->obligations_refuted,
        summary->obligations_unknown,
        jude_result_name(summary->aggregate));
}

bool jude_certificate_sign(const char* canonical_body,
                           const uint8_t* key, size_t key_len,
                           char* out_tag_hex) {
    if (!canonical_body || !key || key_len == 0 || !out_tag_hex) return false;

    uint8_t mac[32];
    nyx_hmac_sha256(key, (uint64_t)key_len,
                    (const uint8_t*)canonical_body, strlen(canonical_body),
                    mac);
    hex_encode(mac, 32, out_tag_hex);
    return true;
}

bool jude_tag_equals(const char* a, const char* b) {
    if (!a || !b) return false;
    volatile unsigned diff = 0;
    for (size_t i = 0; i < 64; i++) {
        diff |= (unsigned)(unsigned char)a[i] ^ (unsigned)(unsigned char)b[i];
        if (a[i] == '\0' || b[i] == '\0') break;
    }
    return diff == 0;
}

bool jude_certificate_verify(const char* canonical_body,
                             const char* presented_tag_hex,
                             const uint8_t* key, size_t key_len,
                             const JudeObligationSummary* summary) {
    if (!canonical_body || !presented_tag_hex || !key || key_len == 0 || !summary) {
        return false;
    }

    /* 1. cryptographic integrity must hold */
    char computed[JUDE_TAG_HEX];
    if (!jude_certificate_sign(canonical_body, key, key_len, computed)) return false;
    if (!jude_tag_equals(computed, presented_tag_hex)) return false;

    /* 2. and the claims inside must be honest: everything proved, nothing
          unknown or contradicted */
    if (summary->aggregate != JUDE_UNSAT) return false;
    if (summary->obligations_proved != summary->obligation_count) return false;
    if (summary->obligation_count == 0) return false;

    return true;
}
