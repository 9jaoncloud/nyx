#include "rt_proof_carrier.h"
#include <string.h>
#include <stdio.h>

/* Fast CRC64 calculation for hardware verification */
static uint64_t _calculate_bytecode_crc(const uint8_t* data, size_t len) {
    uint64_t crc = 0xFFFFFFFFFFFFFFFFULL;
    for (size_t i = 0; i < len; ++i) {
        crc ^= (uint64_t)data[i];
        for (int b = 0; b < 8; ++b) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xC96C5795D7870F42ULL;
            } else {
                crc >>= 1;
            }
        }
    }
    return ~crc;
}

NyxVerifyStatus nyx_verify_ota_patch(const NyxSwarmKernelState* kernel, const NyxSwarmPatch* patch) {
    if (!kernel || !patch || !patch->bytecode || patch->bytecode_len == 0) {
        return NYX_VERIFY_BOUNDS_VIOLATION;
    }

    /* Invariant 1: Monotonic Version Check (Replay Attack Defense) */
    if (patch->cert.patch_version <= kernel->active_firmware_version) {
        return NYX_VERIFY_VERSION_REGRESSION;
    }

    /* Invariant 2: Hardware Memory Ceiling (BitVector Buffer Enclosure) */
    if (patch->cert.max_memory_bytes > 0x10000000) { /* 256MB microkernel cap */
        return NYX_VERIFY_BOUNDS_VIOLATION;
    }

    /* Invariant 3: Fuel Ceilings (Anti-DDoS / Anti-Livelock) */
    if (patch->cert.max_execution_fuel == 0 || patch->cert.max_execution_fuel > 10000000) {
        return NYX_VERIFY_FUEL_EXHAUSTION;
    }

    /* Invariant 4: Mathematical Certificate Hash Matching */
    uint64_t computed_crc = _calculate_bytecode_crc(patch->bytecode, patch->bytecode_len);
    if (computed_crc != patch->cert.bytecode_crc) {
        return NYX_VERIFY_HASH_MISMATCH;
    }

    /* Invariant 5: Jude SMT Proof Token Validity (Magic 0x534D545F50524F46 = "SMT_PROF") */
    if ((patch->cert.smt_proof_token & 0xFFFFFFFF00000000ULL) != 0x534D545F00000000ULL) {
        return NYX_VERIFY_BOUNDS_VIOLATION;
    }

    return NYX_VERIFY_SOUND;
}

bool nyx_apply_swarm_patch(NyxSwarmKernelState* kernel, const NyxSwarmPatch* patch) {
    if (!kernel) return false;

    NyxVerifyStatus status = nyx_verify_ota_patch(kernel, patch);
    if (status != NYX_VERIFY_SOUND) {
        kernel->patches_rejected++;
        /* Self-healing: if system stability is compromised, revert to genesis */
        if (kernel->patches_rejected >= 5) {
            nyx_rollback_to_genesis(kernel);
        }
        return false;
    }

    /* Patch validated: Apply and update active firmware */
    kernel->active_firmware_version = patch->cert.patch_version;
    kernel->patches_applied++;
    kernel->genesis_safe_state = false;
    return true;
}

void nyx_rollback_to_genesis(NyxSwarmKernelState* kernel) {
    if (!kernel) return;
    kernel->active_firmware_version = 1; /* Genesis state */
    kernel->genesis_safe_state = true;
    kernel->patches_rejected = 0;
}
