/*
 * rt_identity.h — Nyx Universal Digital Identity, Passkeys & Selective Disclosure Suite
 *
 * Implements:
 *  1. W3C Verifiable Credentials & Selective-Disclosure JWT (SD-JWT)
 *  2. ICAO Doc 9303 Machine Readable Travel Document (MRTD / e-Passport MRZ)
 *  3. WebAuthn / FIDO2 Passkey Signature Verification
 */

#ifndef RT_IDENTITY_H
#define RT_IDENTITY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Selective Disclosure JWT (SD-JWT) ─────────────────────────────────── */
typedef struct {
    char salt[32];
    char claim_name[32];
    char claim_value[64];
    char disclosure_digest_sha256[65];
} NyxSdJwtDisclosure;

typedef struct {
    char issuer_did[64];
    char subject_did[64];
    size_t disclosure_count;
    NyxSdJwtDisclosure disclosures[8];
} NyxSdJwtToken;

NyxSdJwtToken nyx_identity_create_sd_jwt(const char* issuer, const char* subject);
void nyx_identity_add_disclosure(NyxSdJwtToken* token, const char* claim_name, const char* claim_value);
bool nyx_identity_verify_selective_claim(const NyxSdJwtToken* token, const char* claim_name, const char* expected_value);

/* ── 2. ICAO Doc 9303 Machine Readable Zone (MRZ) Parser ──────────────────── */
typedef struct {
    char document_type[4];      /* "P<" for Passport */
    char issuing_country[4];    /* "NGA", "USA", "GBR", "EST" */
    char surname[32];
    char given_names[32];
    char passport_number[16];
    char nationality[4];
    char date_of_birth[8];      /* YYMMDD */
    char sex[2];                /* 'M', 'F', '<' */
    char expiration_date[8];    /* YYMMDD */
    bool is_checksum_valid;
} NyxIcaoMrzResult;

NyxIcaoMrzResult nyx_identity_parse_icao9303_td3(const char* line1, const char* line2);

/* ── 3. WebAuthn / Passkey Public Key Authenticator ──────────────────────── */
typedef struct {
    char rp_id_hash[65];
    uint32_t sign_count;
    bool user_present;
    bool user_verified;
} NyxWebAuthnAuthData;

NyxWebAuthnAuthData nyx_identity_parse_webauthn_auth_data(const uint8_t* raw_auth_data, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* RT_IDENTITY_H */
