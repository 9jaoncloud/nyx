/**
 * ==============================================================================
 * Nyx Language Ecosystem — Compiler Hardening & Formal Verification Header
 * Module: nyx.hardening
 * ==============================================================================
 */

#ifndef NYX_HARDENING_H
#define NYX_HARDENING_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Phase Ordering Formal Invariance Verifier ────────────────────────── */
typedef enum {
    PASS_INLINE,
    PASS_ESCAPE_ANALYSIS,
    PASS_REGION_LOWERING,
    PASS_VECTORIZE,
    PASS_DCE,
    PASS_CONSTANT_PROP
} NyxPassType;

typedef struct NyxOptimizationPass {
    NyxPassType pass_type;
    const char* pass_name;
    uint32_t pass_order;
    bool modifies_cfg;
    bool preserves_regions;
} NyxOptimizationPass;

typedef struct NyxPhaseOrderProof {
    uint64_t proof_id;
    bool is_semantically_equivalent;
    bool region_invariants_held;
    uint32_t total_passes_verified;
    char proof_summary[256];
} NyxPhaseOrderProof;

NyxPhaseOrderProof nyx_hardening_verify_phase_ordering(const NyxOptimizationPass* passes, size_t pass_count);

/* ── 2. Automated Constant-Time & Timing Leak Armor ─────────────────────── */
typedef struct NyxSecretSymbol {
    const char* symbol_name;
    size_t size_bytes;
    bool is_constant_time_guaranteed;
    uint32_t branchless_lowered_ops;
} NyxSecretSymbol;

typedef struct NyxConstantTimeResult {
    bool passed_verification;
    uint32_t flagged_leaks;
    uint32_t branchless_conversions;
    char report[256];
} NyxConstantTimeResult;

/* Inspects AST / IR and rewrites data-dependent branches to branchless masks (cmov, bitwise selection) */
NyxConstantTimeResult nyx_hardening_enforce_constant_time(const NyxSecretSymbol* secrets, size_t secret_count, uint32_t branch_count);
uint64_t nyx_ct_select_u64(uint64_t mask, uint64_t val_true, uint64_t val_false);
uint32_t nyx_ct_select_u32(uint32_t mask, uint32_t val_true, uint32_t val_false);

/* ── 3. Hardware-Software Co-Verification & Silicon Trojan Detection ─────── */
typedef struct NyxRtlGateNode {
    uint32_t gate_id;
    const char* gate_name;
    bool is_reachable_by_isa;
    bool triggers_privileged_state;
    uint32_t trigger_sequence[4];
    size_t trigger_len;
} NyxRtlGateNode;

typedef struct NyxTrojanScanResult {
    bool is_clean;
    uint32_t total_gates_scanned;
    uint32_t ghost_paths_detected;
    char flagged_trojan_name[128];
    char mitigation_action[256];
} NyxTrojanScanResult;

NyxTrojanScanResult nyx_hardening_scan_silicon_trojans(const NyxRtlGateNode* gates, size_t gate_count, const uint32_t* executed_isa_stream, size_t isa_len);

/* ── 4. Speculative Execution Shield (Surgical Barrier Injection) ────────── */
typedef struct NyxSpeculationReport {
    uint32_t speculative_windows_scanned;
    uint32_t barriers_inserted;
    uint32_t barriers_pruned_safe;
    double estimated_runtime_overhead_pct;
} NyxSpeculationReport;

NyxSpeculationReport nyx_hardening_shield_speculation(uint32_t conditional_branches, uint32_t memory_loads_in_spec_window, bool taint_propagates_to_cache);

#ifdef __cplusplus
}
#endif

#endif /* NYX_HARDENING_H */
