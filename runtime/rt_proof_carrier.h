#ifndef RT_PROOF_CARRIER_H
#define RT_PROOF_CARRIER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum NyxVerifyStatus {
    NYX_VERIFY_SOUND = 0,               /* Proof formally verified: Safe to execute */
    NYX_VERIFY_VERSION_REGRESSION = 1,  /* Monotonic version check failed (replay attack) */
    NYX_VERIFY_BOUNDS_VIOLATION = 2,    /* BitVector index exceeds memory envelope */
    NYX_VERIFY_FUEL_EXHAUSTION = 3,     /* Loop without provable termination invariant */
    NYX_VERIFY_HASH_MISMATCH = 4        /* Bytecode does not match mathematical certificate */
} NyxVerifyStatus;

/* ── Formal Mathematical Proof Header ── */
typedef struct NyxProofCertificate {
    uint32_t patch_version;
    uint32_t max_memory_bytes;     /* BitVector memory ceiling invariant */
    uint32_t max_execution_fuel;   /* Bounded recursion fuel guarantee */
    uint64_t bytecode_crc;         /* Fast hardware polynomial integrity */
    uint64_t smt_proof_token;      /* Jude SMT BitVector certificate token */
} NyxProofCertificate;

/* ── Swarm OTA Firmware Patch ── */
typedef struct NyxSwarmPatch {
    NyxProofCertificate cert;
    uint32_t bytecode_len;
    const uint8_t* bytecode;
} NyxSwarmPatch;

/* ── Microkernel Autonomous State ── */
typedef struct NyxSwarmKernelState {
    uint32_t active_firmware_version;
    uint32_t patches_applied;
    uint32_t patches_rejected;
    bool genesis_safe_state;
} NyxSwarmKernelState;

/* Embedded Verification Engine */
NyxVerifyStatus nyx_verify_ota_patch(const NyxSwarmKernelState* kernel, const NyxSwarmPatch* patch);

/* Self-Healing Execution */
bool nyx_apply_swarm_patch(NyxSwarmKernelState* kernel, const NyxSwarmPatch* patch);

/* Emergency Self-Healing Genesis Rollback */
void nyx_rollback_to_genesis(NyxSwarmKernelState* kernel);

#ifdef __cplusplus
}
#endif

#endif /* RT_PROOF_CARRIER_H */
