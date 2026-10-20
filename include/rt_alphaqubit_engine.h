/*
 * rt_alphaqubit_engine.h — Google Quantum AI AlphaQubit Neural Surface Code Decoder
 *
 * Implements:
 *  1. Quantum Error Correction (QEC) for Distance-3, Distance-5 & Distance-7 Surface Codes
 *  2. Real-Time Neural Syndrome Graph Decoding for Superconducting Qubits
 *  3. Bit-Flip (X) & Phase-Flip (Z) Pauli Error Identification
 *  4. Sub-Threshold Logical Error Rate (LER) Suppression below Physical Threshold
 */

#ifndef RT_ALPHAQUBIT_ENGINE_H
#define RT_ALPHAQUBIT_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ALPHAQUBIT_MAX_SYNDROMES 128

typedef struct {
    uint32_t surface_code_distance; // e.g. 3, 5, 7
    uint32_t num_physical_qubits;   // e.g. 17 (d=3), 49 (d=5), 105 (d=7)
    uint32_t num_rounds;            // QEC measurement cycles
    double physical_error_rate;     // e.g. 0.001 (0.1%)
} AlphaQubitConfig;

typedef struct {
    uint8_t syndrome_measurements[ALPHAQUBIT_MAX_SYNDROMES];
    size_t syndrome_count;
    uint32_t predicted_pauli_x_corrections;
    uint32_t predicted_pauli_z_corrections;
    double logical_error_rate_per_round;
    double decoding_latency_microseconds;
    bool logical_qubit_state_preserved;
} AlphaQubitDecodeResult;

AlphaQubitConfig alphaqubit_create_config(uint32_t distance, double p_phys);
AlphaQubitDecodeResult alphaqubit_decode_syndromes(const AlphaQubitConfig* config, const uint8_t* syndromes, size_t count);

#ifdef __cplusplus
}
#endif

#endif /* RT_ALPHAQUBIT_ENGINE_H */
