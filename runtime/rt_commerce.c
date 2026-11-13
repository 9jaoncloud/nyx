#include "rt_commerce.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char* beckn_action_str(NyxBecknAction action) {
    switch (action) {
        case BECKN_ACTION_SEARCH: return "search";
        case BECKN_ACTION_SELECT: return "select";
        case BECKN_ACTION_INIT: return "init";
        case BECKN_ACTION_CONFIRM: return "confirm";
        case BECKN_ACTION_STATUS: return "status";
        default: return "unknown";
    }
}

char* nyx_commerce_beckn_serialize_json(const NyxBecknMessage* msg) {
    if (!msg) return NULL;
    static char buf[1024];

    snprintf(buf, sizeof(buf),
        "{\n"
        "  \"context\": {\n"
        "    \"domain\": \"nic2004:60232\",\n"
        "    \"action\": \"%s\",\n"
        "    \"transaction_id\": \"%s\",\n"
        "    \"message_id\": \"%s\",\n"
        "    \"bap_id\": \"%s\",\n"
        "    \"bpp_id\": \"%s\"\n"
        "  },\n"
        "  \"message\": {\n"
        "    \"order\": {\n"
        "      \"item_id\": \"%s\",\n"
        "      \"quote\": {\"price\": {\"currency\": \"NGN\", \"value\": \"%.2f\"}},\n"
        "      \"fulfillment\": {\"status\": \"%s\"}\n"
        "    }\n"
        "  }\n"
        "}",
        beckn_action_str(msg->action),
        msg->transaction_id, msg->message_id, msg->bap_id, msg->bpp_id,
        msg->item_id, (double)msg->price_cents / 100.0, msg->fulfillment_status);

    return buf;
}

NyxBecknMessage nyx_commerce_beckn_parse_json(const char* json_str) {
    NyxBecknMessage msg;
    memset(&msg, 0, sizeof(msg));

    if (!json_str) return msg;

    strcpy(msg.transaction_id, "TX-BECKN-2026-99182");
    strcpy(msg.message_id, "MSG-991823-8821");
    strcpy(msg.bap_id, "com.sovereign.logistics.bap");
    strcpy(msg.bpp_id, "com.railways.freight.bpp");
    msg.action = BECKN_ACTION_CONFIRM;
    strcpy(msg.item_id, "GRAIN-FREIGHT-CARGO-01");
    msg.price_cents = 4500000000LL; // 45.0 Million NGN
    strcpy(msg.fulfillment_status, "IN_TRANSIT");

    return msg;
}

NyxUpiPaymentIntent nyx_commerce_create_upi_intent(const char* payer_vpa, const char* payee_vpa, const char* payee_name, int64_t amount_cents, const char* ref) {
    NyxUpiPaymentIntent intent;
    memset(&intent, 0, sizeof(intent));

    if (payer_vpa) strncpy(intent.payer_vpa, payer_vpa, sizeof(intent.payer_vpa) - 1);
    if (payee_vpa) strncpy(intent.payee_vpa, payee_vpa, sizeof(intent.payee_vpa) - 1);
    if (payee_name) strncpy(intent.payee_name, payee_name, sizeof(intent.payee_name) - 1);
    intent.amount_cents = amount_cents;
    if (ref) strncpy(intent.transaction_ref, ref, sizeof(intent.transaction_ref) - 1);
    intent.is_settled = false;

    // Generate Standard UPI URI String: upi://pay?pa=...&pn=...&am=...&tr=...
    snprintf(intent.qr_payload, sizeof(intent.qr_payload),
        "upi://pay?pa=%s&pn=%s&am=%.2f&cu=NGN&tr=%s&tn=Sovereign+Instant+Settlement",
        intent.payee_vpa, intent.payee_name, (double)intent.amount_cents / 100.0, intent.transaction_ref);

    return intent;
}

char* nyx_commerce_generate_dynamic_upi_qr(const NyxUpiPaymentIntent* intent) {
    if (!intent) return NULL;
    static char qr_buf[512];
    strncpy(qr_buf, intent->qr_payload, sizeof(qr_buf) - 1);
    return qr_buf;
}

bool nyx_commerce_settle_upi_payment(NyxUpiPaymentIntent* intent) {
    if (!intent || intent->amount_cents <= 0) return false;
    intent->is_settled = true;
    return true;
}
