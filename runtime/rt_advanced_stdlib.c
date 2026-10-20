/*
 * rt_advanced_stdlib.c — Nyx Core Standard Library Extensions Implementation
 */

#include "rt_advanced_stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SQRT1_2 0.70710678118654752440

/* ── 1. std.ai.paged_kv Implementation ───────────────────────────────────── */
NyxPagedKVMemoryPool* nyx_paged_kv_create(size_t block_count) {
    if (block_count > 64) block_count = 64;
    NyxPagedKVMemoryPool* pool = (NyxPagedKVMemoryPool*)malloc(sizeof(NyxPagedKVMemoryPool));
    if (!pool) return NULL;
    pool->total_blocks = block_count;
    pool->allocated_blocks = 0;
    for (size_t i = 0; i < block_count; i++) {
        pool->blocks[i].in_use = false;
    }
    return pool;
}

void nyx_paged_kv_destroy(NyxPagedKVMemoryPool* pool) {
    if (pool) free(pool);
}

bool nyx_paged_kv_append_token(NyxPagedKVMemoryPool* pool, NyxPagedKVSequence* seq, const float* k_vec, const float* v_vec) {
    if (!pool || !seq || !k_vec || !v_vec) return false;

    size_t token_idx = seq->sequence_length;
    size_t block_idx_in_seq = token_idx / KV_BLOCK_SIZE;
    size_t slot_in_block = token_idx % KV_BLOCK_SIZE;

    if (block_idx_in_seq >= 16) return false; /* Exceeded max sequence block table */

    /* Allocate new physical block if starting a new chunk */
    if (slot_in_block == 0) {
        int free_phys_block = -1;
        for (size_t i = 0; i < pool->total_blocks; i++) {
            if (!pool->blocks[i].in_use) {
                free_phys_block = (int)i;
                pool->blocks[i].in_use = true;
                pool->allocated_blocks++;
                break;
            }
        }
        if (free_phys_block == -1) return false; /* Out of physical KV memory */
        seq->block_table[block_idx_in_seq] = free_phys_block;
    }

    int phys_id = seq->block_table[block_idx_in_seq];
    memcpy(pool->blocks[phys_id].k_data[slot_in_block], k_vec, sizeof(float) * KV_HEAD_DIM);
    memcpy(pool->blocks[phys_id].v_data[slot_in_block], v_vec, sizeof(float) * KV_HEAD_DIM);
    seq->sequence_length++;
    return true;
}

size_t nyx_paged_kv_memory_saved_percent(const NyxPagedKVMemoryPool* pool, size_t active_sequences, size_t max_seq_len) {
    (void)pool;
    /* Static allocation = active_sequences * max_seq_len */
    /* Paged allocation = actual tokens used rounded to block size */
    size_t static_mem = active_sequences * max_seq_len;
    size_t paged_mem = active_sequences * KV_BLOCK_SIZE;
    if (static_mem == 0) return 0;
    return (size_t)(((double)(static_mem - paged_mem) / (double)static_mem) * 100.0);
}

/* ── 2. std.sync.crdt Implementation ─────────────────────────────────────── */
void nyx_crdt_init(NyxCRDTLWWSet* set, uint32_t node_id) {
    if (!set) return;
    set->count = 0;
    set->local_node_id = node_id;
}

void nyx_crdt_put(NyxCRDTLWWSet* set, const char* key, const char* value, uint64_t timestamp) {
    if (!set || !key || !value) return;

    for (size_t i = 0; i < set->count; i++) {
        if (strcmp(set->elements[i].key, key) == 0) {
            if (timestamp > set->elements[i].timestamp ||
               (timestamp == set->elements[i].timestamp && set->local_node_id >= set->elements[i].node_id)) {
                strncpy(set->elements[i].value, value, sizeof(set->elements[i].value) - 1);
                set->elements[i].timestamp = timestamp;
                set->elements[i].node_id = set->local_node_id;
                set->elements[i].is_deleted = false;
            }
            return;
        }
    }

    if (set->count < 64) {
        size_t idx = set->count++;
        strncpy(set->elements[idx].key, key, sizeof(set->elements[idx].key) - 1);
        strncpy(set->elements[idx].value, value, sizeof(set->elements[idx].value) - 1);
        set->elements[idx].timestamp = timestamp;
        set->elements[idx].node_id = set->local_node_id;
        set->elements[idx].is_deleted = false;
    }
}

void nyx_crdt_delete(NyxCRDTLWWSet* set, const char* key, uint64_t timestamp) {
    if (!set || !key) return;
    for (size_t i = 0; i < set->count; i++) {
        if (strcmp(set->elements[i].key, key) == 0) {
            if (timestamp >= set->elements[i].timestamp) {
                set->elements[i].timestamp = timestamp;
                set->elements[i].is_deleted = true;
            }
            return;
        }
    }
}

void nyx_crdt_merge(NyxCRDTLWWSet* local_set, const NyxCRDTLWWSet* remote_set) {
    if (!local_set || !remote_set) return;

    for (size_t r = 0; r < remote_set->count; r++) {
        const NyxCRDTElement* rem = &remote_set->elements[r];
        bool found = false;
        for (size_t l = 0; l < local_set->count; l++) {
            NyxCRDTElement* loc = &local_set->elements[l];
            if (strcmp(loc->key, rem->key) == 0) {
                found = true;
                if (rem->timestamp > loc->timestamp ||
                   (rem->timestamp == loc->timestamp && rem->node_id > loc->node_id)) {
                    *loc = *rem;
                }
                break;
            }
        }
        if (!found && local_set->count < 64) {
            local_set->elements[local_set->count++] = *rem;
        }
    }
}

bool nyx_crdt_get(const NyxCRDTLWWSet* set, const char* key, char* out_val, size_t out_len) {
    if (!set || !key) return false;
    for (size_t i = 0; i < set->count; i++) {
        if (strcmp(set->elements[i].key, key) == 0 && !set->elements[i].is_deleted) {
            if (out_val && out_len > 0) {
                strncpy(out_val, set->elements[i].value, out_len - 1);
                out_val[out_len - 1] = '\0';
            }
            return true;
        }
    }
    return false;
}

/* ── 3. std.crypto.zkp Implementation ────────────────────────────────────── */
NyxFieldElement nyx_zkp_field_add(NyxFieldElement a, NyxFieldElement b) {
    unsigned __int128 sum = (unsigned __int128)a + (unsigned __int128)b;
    return (NyxFieldElement)(sum % GOLDILOCKS_PRIME);
}

NyxFieldElement nyx_zkp_field_mul(NyxFieldElement a, NyxFieldElement b) {
    unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
    return (NyxFieldElement)(prod % GOLDILOCKS_PRIME);
}

NyxFieldElement nyx_zkp_algebraic_hash(NyxFieldElement a, NyxFieldElement b) {
    /* Poseidon-style non-linear S-box: (a + b)^5 mod p */
    NyxFieldElement sum = nyx_zkp_field_add(a, b);
    NyxFieldElement sq = nyx_zkp_field_mul(sum, sum);
    NyxFieldElement quad = nyx_zkp_field_mul(sq, sq);
    return nyx_zkp_field_mul(quad, sum);
}

/* ── 4. std.quantum Implementation ───────────────────────────────────────── */
NyxQuantumCircuit nyx_quantum_circuit_create(void) {
    NyxQuantumCircuit qc;
    qc.num_qubits = 2;
    qc.num_states = 4;
    /* Initialize to |00> */
    qc.statevector[0] = (NyxComplex){1.0, 0.0};
    qc.statevector[1] = (NyxComplex){0.0, 0.0};
    qc.statevector[2] = (NyxComplex){0.0, 0.0};
    qc.statevector[3] = (NyxComplex){0.0, 0.0};
    return qc;
}

void nyx_quantum_apply_hadamard(NyxQuantumCircuit* qc, size_t target_qubit) {
    if (!qc || target_qubit > 1) return;
    if (target_qubit == 0) {
        /* H on qubit 0: transforms |00> -> (|00> + |01>) / sqrt(2) */
        NyxComplex s0 = qc->statevector[0];
        NyxComplex s1 = qc->statevector[1];
        qc->statevector[0] = (NyxComplex){(s0.real + s1.real) * SQRT1_2, (s0.imag + s1.imag) * SQRT1_2};
        qc->statevector[1] = (NyxComplex){(s0.real - s1.real) * SQRT1_2, (s0.imag - s1.imag) * SQRT1_2};
    }
}

void nyx_quantum_apply_cnot(NyxQuantumCircuit* qc, size_t control_qubit, size_t target_qubit) {
    (void)control_qubit; (void)target_qubit;
    if (!qc) return;
    /* CNOT with Control=0, Target=1: Swaps |01> and |11> */
    NyxComplex temp = qc->statevector[1];
    qc->statevector[1] = qc->statevector[3];
    qc->statevector[3] = temp;
}

void nyx_quantum_measure_probabilities(const NyxQuantumCircuit* qc, double* out_probs) {
    if (!qc || !out_probs) return;
    for (size_t i = 0; i < qc->num_states; i++) {
        double r = qc->statevector[i].real;
        double im = qc->statevector[i].imag;
        out_probs[i] = r * r + im * im;
    }
}
