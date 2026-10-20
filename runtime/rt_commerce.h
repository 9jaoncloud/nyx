/*
 * rt_commerce.h — Nyx Universal Decentralized Commerce & Instant Payment Rail Suite
 *
 * Implements:
 *  1. Beckn Open Protocol: Decentralized Commerce JSON Schema (Search, Select, Init, Confirm)
 *  2. UPI-Style Tokenized Instant Payment Switch: VPA (Virtual Payment Address) Routing & Dynamic QR
 */

#ifndef RT_COMMERCE_H
#define RT_COMMERCE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Beckn Decentralized Commerce Protocol ─────────────────────────────── */
typedef enum {
    BECKN_ACTION_SEARCH,
    BECKN_ACTION_SELECT,
    BECKN_ACTION_INIT,
    BECKN_ACTION_CONFIRM,
    BECKN_ACTION_STATUS
} NyxBecknAction;

typedef struct {
    char transaction_id[64];
    char message_id[64];
    char bap_id[64];            /* Buyer App ID e.g. "com.myapp.buyer" */
    char bpp_id[64];            /* Seller App ID e.g. "com.transit.rail" */
    NyxBecknAction action;
    char item_id[32];           /* e.g. "GRAIN-FREIGHT-CARGO-01" */
    int64_t price_cents;
    char fulfillment_status[32];/* "IN_TRANSIT", "DELIVERED" */
} NyxBecknMessage;

char* nyx_commerce_beckn_serialize_json(const NyxBecknMessage* msg);
NyxBecknMessage nyx_commerce_beckn_parse_json(const char* json_str);

/* ── 2. UPI-Style Instant Tokenized Payment Switch ────────────────────────── */
typedef struct {
    char payer_vpa[64];         /* e.g. "tariq@cbn" */
    char payee_vpa[64];         /* e.g. "nationalhospital@fbn" */
    char payee_name[64];
    int64_t amount_cents;
    char transaction_ref[32];
    char qr_payload[256];
    bool is_settled;
} NyxUpiPaymentIntent;

NyxUpiPaymentIntent nyx_commerce_create_upi_intent(const char* payer_vpa, const char* payee_vpa, const char* payee_name, int64_t amount_cents, const char* ref);
char* nyx_commerce_generate_dynamic_upi_qr(const NyxUpiPaymentIntent* intent);
bool nyx_commerce_settle_upi_payment(NyxUpiPaymentIntent* intent);

#ifdef __cplusplus
}
#endif

#endif /* RT_COMMERCE_H */
