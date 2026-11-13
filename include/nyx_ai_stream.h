/**
 * ==============================================================================
 * Nyx Language Ecosystem — Machine Learning & Hardware Acceleration Header
 * Module: nyx.ai_stream
 * ==============================================================================
 */

#ifndef NYX_AI_STREAM_H
#define NYX_AI_STREAM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 9. Zero-VRAM Edge Weight Streaming Module (Fixed 64MB Window) ───────── */
typedef struct NyxTensorLayer {
    uint32_t layer_id;
    size_t weight_size_bytes;
    uint32_t input_dim;
    uint32_t output_dim;
    bool is_quantized_q4;
} NyxTensorLayer;

typedef struct NyxStreamInferResult {
    bool success;
    size_t total_model_bytes;
    size_t peak_ram_used_bytes;
    double throughput_tokens_per_sec;
    char status[256];
} NyxStreamInferResult;

/* Executes a multi-gigabyte model within a strict fixed memory window */
NyxStreamInferResult nyx_ai_stream_forward_pass(const NyxTensorLayer* layers, size_t layer_count, size_t max_ram_window_bytes);

/* ── 10. Neural Symbolic Decompiler (Decision Tree Extractor) ────────────── */
typedef struct NyxSymbolicDecisionNode {
    uint32_t node_id;
    const char* feature_name;
    double threshold;
    uint32_t true_branch_child;
    uint32_t false_branch_child;
    bool is_leaf;
    int32_t predicted_class;
} NyxSymbolicDecisionNode;

typedef struct NyxDecompileReport {
    uint32_t total_extracted_rules;
    double fidelity_score_pct;
    bool is_formally_verifiable;
    char generated_nyx_dsl[512];
} NyxDecompileReport;

NyxDecompileReport nyx_ai_decompile_to_symbolic(const float* weight_matrix, size_t rows, size_t cols);

/* ── 11. Opto-Electronic & Photonic Lowering Pass ─────────────────────────── */
typedef struct NyxPhotonicPhaseShift {
    uint32_t waveguide_id;
    double phase_radians;
    double thermal_drift_offset_rad;
    double compensated_phase_rad;
} NyxPhotonicPhaseShift;

typedef struct NyxPhotonicCompileResult {
    uint32_t total_waveguides;
    double total_energy_pj_per_mac;
    bool thermal_drift_stabilized;
} NyxPhotonicCompileResult;

NyxPhotonicCompileResult nyx_ai_compile_photonic_mesh(NyxPhotonicPhaseShift* shifts, size_t count, double ambient_temp_celsius);

/* ── 12. Processing-In-Memory (PIM) Kernel Synthesis ─────────────────────── */
typedef struct NyxPimKernelConfig {
    uint32_t dram_banks_utilized;
    size_t vector_length;
    bool executes_in_dram_die;
    double energy_saved_vs_von_neumann_pct;
} NyxPimKernelConfig;

NyxPimKernelConfig nyx_ai_synthesize_pim_kernel(size_t matrix_size, uint32_t available_dram_banks);

#ifdef __cplusplus
}
#endif

#endif /* NYX_AI_STREAM_H */
