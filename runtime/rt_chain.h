#ifndef RT_CHAIN_H
#define RT_CHAIN_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rt_string.h"
#include "rt_crypto.h"

/* ── 1. Merkle Tree Construction & Proof Verification ── */

typedef struct {
    uint8_t hash[32];
} NyxHash256;

static inline void rt_merkle_combine(const uint8_t* left, const uint8_t* right, uint8_t* parent_out) {
    uint8_t concat[64];
    memcpy(concat, left, 32);
    memcpy(concat + 32, right, 32);
    nyx_sha256(concat, 64, parent_out);
}

static inline bool rt_merkle_verify_proof(
    const uint8_t* leaf_hash,
    const uint8_t* proof_hashes,
    const bool* proof_is_right,
    size_t proof_len,
    const uint8_t* expected_root
) {
    uint8_t current[32];
    memcpy(current, leaf_hash, 32);

    for (size_t i = 0; i < proof_len; i++) {
        const uint8_t* sibling = proof_hashes + (i * 32);
        if (proof_is_right[i]) {
            rt_merkle_combine(current, sibling, current);
        } else {
            rt_merkle_combine(sibling, current, current);
        }
    }
    return memcmp(current, expected_root, 32) == 0;
}

/* ── 2. UTXO & Block Header Structure ── */

typedef struct {
    uint32_t version;
    uint8_t prev_block_hash[32];
    uint8_t merkle_root[32];
    uint64_t timestamp;
    uint32_t difficulty_target;
    uint64_t nonce;
    uint64_t transaction_count;
} NyxBlockHeader;

/* ── 3. Practical Byzantine Fault Tolerance (PBFT) State Engine ── */

typedef enum {
    PBFT_PRE_PREPARE = 0,
    PBFT_PREPARE = 1,
    PBFT_COMMIT = 2,
    PBFT_COMMITTED = 3
} NyxPbftPhase;

typedef struct {
    uint64_t view_number;
    uint64_t sequence_number;
    NyxPbftPhase phase;
    int total_nodes;
    int faulty_threshold; // f = (total - 1) / 3
    int prepare_votes;
    int commit_votes;
} NyxPbftConsensusState;

static inline NyxPbftConsensusState rt_pbft_init(int total_nodes) {
    NyxPbftConsensusState s;
    s.view_number = 1;
    s.sequence_number = 1;
    s.phase = PBFT_PRE_PREPARE;
    s.total_nodes = total_nodes;
    s.faulty_threshold = (total_nodes - 1) / 3;
    s.prepare_votes = 1;
    s.commit_votes = 0;
    return s;
}

static inline bool rt_pbft_receive_prepare_vote(NyxPbftConsensusState* s) {
    s->prepare_votes++;
    int quorum = 2 * s->faulty_threshold + 1;
    if (s->prepare_votes >= quorum && s->phase == PBFT_PRE_PREPARE) {
        s->phase = PBFT_PREPARE;
        return true;
    }
    return false;
}

static inline bool rt_pbft_receive_commit_vote(NyxPbftConsensusState* s) {
    s->commit_votes++;
    int quorum = 2 * s->faulty_threshold + 1;
    if (s->commit_votes >= quorum && s->phase == PBFT_PREPARE) {
        s->phase = PBFT_COMMITTED;
        return true;
    }
    return false;
}

#endif // RT_CHAIN_H
