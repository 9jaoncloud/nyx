#include "rt_identity.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* Simple SHA-256 Mock / Digest Generator for deterministic SD-JWT testing */
static void compute_mock_sha256(const char* input, char* out_hex) {
    uint32_t hash = 5381;
    for (size_t i = 0; input[i] != '\0'; i++) {
        hash = ((hash << 5) + hash) + (uint8_t)input[i];
    }
    snprintf(out_hex, 65, "sha256_%08x%08x%08x%08x", hash, hash ^ 0x5a5a5a5a, hash + 0x1234, hash ^ 0xa5a5a5a5);
}

NyxSdJwtToken nyx_identity_create_sd_jwt(const char* issuer, const char* subject) {
    NyxSdJwtToken token;
    memset(&token, 0, sizeof(token));
    if (issuer) strncpy(token.issuer_did, issuer, sizeof(token.issuer_did) - 1);
    if (subject) strncpy(token.subject_did, subject, sizeof(token.subject_did) - 1);
    token.disclosure_count = 0;
    return token;
}

void nyx_identity_add_disclosure(NyxSdJwtToken* token, const char* claim_name, const char* claim_value) {
    if (!token || token->disclosure_count >= 8 || !claim_name || !claim_value) return;

    NyxSdJwtDisclosure* d = &token->disclosures[token->disclosure_count++];
    snprintf(d->salt, sizeof(d->salt), "salt_%04x", (unsigned int)token->disclosure_count);
    strncpy(d->claim_name, claim_name, sizeof(d->claim_name) - 1);
    strncpy(d->claim_value, claim_value, sizeof(d->claim_value) - 1);

    char combined[128];
    snprintf(combined, sizeof(combined), "[\"%s\",\"%s\",\"%s\"]", d->salt, d->claim_name, d->claim_value);
    compute_mock_sha256(combined, d->disclosure_digest_sha256);
}

bool nyx_identity_verify_selective_claim(const NyxSdJwtToken* token, const char* claim_name, const char* expected_value) {
    if (!token || !claim_name) return false;

    for (size_t i = 0; i < token->disclosure_count; i++) {
        if (strcmp(token->disclosures[i].claim_name, claim_name) == 0) {
            if (!expected_value) return true; // Claim exists
            return strcmp(token->disclosures[i].claim_value, expected_value) == 0;
        }
    }
    return false;
}

static int icao_char_weight(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    return 0; // '<'
}

static bool verify_mrz_checksum(const char* data, size_t len, char check_digit) {
    static const int weights[3] = {7, 3, 1};
    int sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum += icao_char_weight(data[i]) * weights[i % 3];
    }
    int expected = sum % 10;
    int actual = (check_digit >= '0' && check_digit <= '9') ? (check_digit - '0') : 0;
    return expected == actual;
}

NyxIcaoMrzResult nyx_identity_parse_icao9303_td3(const char* line1, const char* line2) {
    NyxIcaoMrzResult res;
    memset(&res, 0, sizeof(res));

    if (!line1 || !line2 || strlen(line1) < 44 || strlen(line2) < 44) {
        res.is_checksum_valid = false;
        return res;
    }

    // Line 1: P<NGAABUBAKAR<<TARIQ<<<<<<<<<<<<<<<<<<<<<<<
    strncpy(res.document_type, line1, 2);
    strncpy(res.issuing_country, line1 + 2, 3);

    const char* names_start = line1 + 5;
    const char* sep = strstr(names_start, "<<");
    if (sep) {
        size_t surname_len = sep - names_start;
        if (surname_len >= sizeof(res.surname)) surname_len = sizeof(res.surname) - 1;
        strncpy(res.surname, names_start, surname_len);

        const char* given_start = sep + 2;
        size_t gi = 0;
        while (given_start[gi] != '<' && gi < sizeof(res.given_names) - 1 && given_start[gi] != '\0') {
            res.given_names[gi] = given_start[gi];
            gi++;
        }
    }

    // Line 2: A123456780NGA8806152M2906154<<<<<<<<<<<<<<02
    strncpy(res.passport_number, line2, 9);
    strncpy(res.nationality, line2 + 10, 3);
    strncpy(res.date_of_birth, line2 + 13, 6);
    res.sex[0] = line2[20];
    strncpy(res.expiration_date, line2 + 21, 6);

    // Verify passport number check digit (pos 9) and DOB check digit (pos 19)
    bool p_ok = verify_mrz_checksum(line2, 9, line2[9]);
    bool dob_ok = verify_mrz_checksum(line2 + 13, 6, line2[19]);
    res.is_checksum_valid = (p_ok && dob_ok);

    return res;
}

NyxWebAuthnAuthData nyx_identity_parse_webauthn_auth_data(const uint8_t* raw_auth_data, size_t length) {
    NyxWebAuthnAuthData out;
    memset(&out, 0, sizeof(out));

    if (!raw_auth_data || length < 37) return out;

    // RP ID Hash: first 32 bytes
    for (int i = 0; i < 32; i++) {
        snprintf(out.rp_id_hash + (i * 2), 3, "%02x", raw_auth_data[i]);
    }

    uint8_t flags = raw_auth_data[32];
    out.user_present = (flags & 0x01) != 0;
    out.user_verified = (flags & 0x04) != 0;

    out.sign_count = (raw_auth_data[33] << 24) | (raw_auth_data[34] << 16) | (raw_auth_data[35] << 8) | raw_auth_data[36];
    return out;
}
