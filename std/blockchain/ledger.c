#include "runtime/rt_db.h"
#include "runtime/rt_stdlib.h"
#include <math.h>
#include "runtime/rt_option.h"
#include "runtime/rt_async.h"
#include <stdlib.h>
#include <string.h>
#include "runtime/rt_concurrency.h"
#include "runtime/rt_map.h"
#include <stdio.h>
#include "runtime/rt_io.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_string.h"
#include "runtime/rt_http.h"
#include "runtime/rt_json.h"
#include "runtime/rt_arena.h"
#include <stdint.h>
#ifndef NYX_TUPLE_DEFS
#define NYX_TUPLE_DEFS
typedef struct { void* f0; void* f1; } NyxTuple2;
typedef struct { void* f0; void* f1; void* f2; } NyxTuple3;
typedef struct { void* f0; void* f1; void* f2; void* f3; } NyxTuple4;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; } NyxTuple5;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; } NyxTuple6;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; } NyxTuple7;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; void* f7; } NyxTuple8;
#endif

typedef struct {
    int64_t height;
    rt_string_t prev_hash;
    rt_string_t merkle_root;
    int64_t tx_count;
    int64_t nonce;
    int64_t timestamp;
} Block;

typedef struct {
    rt_string_t chain_id;
    int64_t block_height;
    int64_t pending_tx_count;
    rt_string_t latest_block_hash;
} BlockchainLedger;

typedef struct {
    int64_t total_nodes;
    int64_t faulty_threshold;
    int64_t phase;
    int64_t prepare_votes;
    int64_t commit_votes;
} PbftConsensus;

BlockchainLedger create_ledger(rt_string_t);
void add_transaction(BlockchainLedger*, rt_string_t, rt_string_t, rt_string_t, double);
Block mine_block(BlockchainLedger*, int64_t);
PbftConsensus create_pbft(int64_t);
int pbft_vote_prepare(PbftConsensus*);
int pbft_vote_commit(PbftConsensus*);
// module ledger
// import std::io
// import std::string
// import std::vec
BlockchainLedger create_ledger(rt_string_t chain_id) {
return (BlockchainLedger){.chain_id = chain_id, .block_height = 1LL, .pending_tx_count = 0LL, .latest_block_hash = rt_string_from("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f")};
}

void add_transaction(BlockchainLedger* chain, rt_string_t tx_id, rt_string_t sender, rt_string_t recipient, double amount) {
(chain->pending_tx_count = ((*chain).pending_tx_count + 1LL));
}

Block mine_block(BlockchainLedger* chain, int64_t nonce) {
(chain->block_height = ((*chain).block_height + 1LL));
int64_t txs = (*chain).pending_tx_count;
(chain->pending_tx_count = 0LL);
Block b = (Block){.height = (*chain).block_height, .prev_hash = ({ rt_string_t _c = (*chain).latest_block_hash; rt_string_retain(&_c); _c; }), .merkle_root = rt_string_from("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"), .tx_count = txs, .nonce = nonce, .timestamp = 1724520000LL};
(chain->latest_block_hash = rt_string_from("00000000000000000001a7c8d9e3f2b1a4c5d6e7f8a9b0c1d2e3f4a5b6c7d8e9"));
return b;
}

PbftConsensus create_pbft(int64_t total_nodes) {
int64_t f = ((total_nodes - 1LL) / 3LL);
return (PbftConsensus){.total_nodes = total_nodes, .faulty_threshold = f, .phase = 0LL, .prepare_votes = 1LL, .commit_votes = 0LL};
}

int pbft_vote_prepare(PbftConsensus* pbft) {
(pbft->prepare_votes = ((*pbft).prepare_votes + 1LL));
int64_t quorum = ((2LL * (*pbft).faulty_threshold) + 1LL);
if ((((*pbft).prepare_votes >= quorum) && ((*pbft).phase == 0LL))) {
(pbft->phase = 1LL);
return 1;
} else {
return 0;
}
}

int pbft_vote_commit(PbftConsensus* pbft) {
(pbft->commit_votes = ((*pbft).commit_votes + 1LL));
int64_t quorum = ((2LL * (*pbft).faulty_threshold) + 1LL);
if ((((*pbft).commit_votes >= quorum) && ((*pbft).phase == 1LL))) {
(pbft->phase = 3LL);
return 1;
} else {
return 0;
}
}

