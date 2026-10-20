#ifndef RT_FORMAL_VERIFY_H
#define RT_FORMAL_VERIFY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Enterprise Formal Verification Engine (Hoare Logic & SMT2) ── */
typedef struct NyxFormalContract {
    const char* function_name;
    bool (*precondition_fn)(const void* args);
    bool (*postcondition_fn)(const void* args, const void* result);
    bool (*invariant_fn)(const void* state);
} NyxFormalContract;

typedef struct NyxVerificationProof {
    uint64_t proof_id;
    const char* theorem_name;
    bool is_verified;
    uint64_t verification_time_ns;
    char smt2_formula[512];
} NyxVerificationProof;

/* Formal Contract & Region Invariant Checking */
NyxVerificationProof nyx_formal_verify_region_soundness(const char* scope_name, size_t region_size, bool has_escape);
bool nyx_formal_check_precondition(const char* fn_name, bool condition, const char* expr_str);
bool nyx_formal_check_postcondition(const char* fn_name, bool condition, const char* expr_str);
bool nyx_formal_check_invariant(const char* struct_name, bool condition, const char* expr_str);

/* ── 2. Global Decentralized Package Registry (Air-Gapped & Cryptographic) ── */
typedef struct NyxPackageManifest {
    char name[64];
    char version[32];
    char author_pubkey[65];     /* Ed25519 Public Key */
    char content_hash[65];      /* SHA-256 Content-Addressed Hash */
    char signature[129];        /* Ed25519 Digital Signature */
    size_t payload_size;
    bool is_verified;
} NyxPackageManifest;

typedef struct NyxRegistryNode {
    char node_id[64];
    char endpoint[128];
    bool is_active;
    int synced_packages_count;
} NyxRegistryNode;

/* Registry Signing, Publishing & Peer Discovery */
NyxPackageManifest nyx_registry_sign_package(const char* name, const char* version, const void* payload, size_t size, const char* private_key);
bool nyx_registry_verify_package(const NyxPackageManifest* manifest, const void* payload, size_t size);
bool nyx_registry_replicate_peer(const NyxPackageManifest* manifest, const char* peer_endpoint);

#ifdef __cplusplus
}
#endif

#endif /* RT_FORMAL_VERIFY_H */
