/**
 * @file nyx_pidgin.h
 * @brief Nigerian Pidgin (Naija) Natural Language & Sovereign Market Intelligence Engine
 * @author Simeon Bala (9jaonCloud Engineering & Sovereign Computing Initiative)
 * 
 * Production-grade morphological analyzer, grammar tokenizer, commercial idiom parser,
 * code-switching bridge (Yoruba, Hausa, Igbo), and voice ledger transaction synthesizer.
 */

#ifndef NYX_PIDGIN_H
#define NYX_PIDGIN_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Transaction & Intent Types */
typedef enum {
    PIDGIN_INTENT_UNKNOWN = 0,
    PIDGIN_INTENT_SALE,               /* "I don sell 3 mudu garri 1500" */
    PIDGIN_INTENT_PURCHASE_EXPENSE,   /* "I buy 5 bag rice from depot" */
    PIDGIN_INTENT_CREDIT_SALE,        /* "Mama Chinedu collect on credit / gbese" */
    PIDGIN_INTENT_DEBT_REPAYMENT,     /* "Baba Tunde don pay in debt complete" */
    PIDGIN_INTENT_QUERY_SUMMARY,      /* "Read my ledger" / "Wetin be my total sales" */
    PIDGIN_INTENT_QUERY_DEBTORS,      /* "Who dey owe me money?" */
    PIDGIN_INTENT_QUERY_INVENTORY,    /* "How many bag beans remain?" */
    PIDGIN_INTENT_CONFIRMATION_YES,   /* "Yes" / "Na so" / "Correct" / "Ehen" */
    PIDGIN_INTENT_CONFIRMATION_NO     /* "No" / "E no correct" / "Rara" */
} NyxPidginIntentType;

/* Measurement Units */
typedef enum {
    PIDGIN_UNIT_ITEM = 0,
    PIDGIN_UNIT_MUDU,       /* Mudu bowl */
    PIDGIN_UNIT_DERICA,     /* Derica tin */
    PIDGIN_UNIT_PAINTER,    /* 4-liter paint bucket */
    PIDGIN_UNIT_KONGO,      /* Kongo bowl */
    PIDGIN_UNIT_BAG,        /* 50kg/100kg bag */
    PIDGIN_UNIT_HALF_BAG,   /* Half bag */
    PIDGIN_UNIT_TIN,        /* Tin can */
    PIDGIN_UNIT_CARTON,     /* Carton / pack */
    PIDGIN_UNIT_CRATE,      /* Crate (drinks, eggs) */
    PIDGIN_UNIT_RUBBER,     /* Small bucket */
    PIDGIN_UNIT_TIE,        /* Tie / bundle */
    PIDGIN_UNIT_PIECE,      /* Single piece */
    PIDGIN_UNIT_TUBER       /* Yam tuber / head */
} NyxPidginUnit;

/* Payment Modes */
typedef enum {
    PIDGIN_PAYMENT_CASH = 0,
    PIDGIN_PAYMENT_TRANSFER,
    PIDGIN_PAYMENT_CREDIT_UNPAID,
    PIDGIN_PAYMENT_PARTIAL
} NyxPidginPaymentMode;

/* Language Dialect / Code-Switch Tag */
typedef enum {
    PIDGIN_DIALECT_STANDARD_NAIJA = 0,
    PIDGIN_DIALECT_YORUBA_MIXED,       /* Yoruba code-switch: "ba mi ta", "owo" */
    PIDGIN_DIALECT_HAUSA_MIXED,        /* Hausa code-switch: "kudi", "nawa ne" */
    PIDGIN_DIALECT_IGBO_MIXED          /* Igbo code-switch: "ego", "biko" */
} NyxPidginDialect;

/* Parsed Transaction Entity */
typedef struct {
    NyxPidginIntentType intent;
    NyxPidginDialect dialect;
    char item_name[64];
    double quantity;
    NyxPidginUnit unit;
    char unit_name[32];
    double unit_price;
    double total_amount;
    char customer_name[64];
    NyxPidginPaymentMode payment_mode;
    double confidence;             /* 0.0 to 1.0 */
    bool needs_confirmation;       /* True if ambiguous or low confidence */
    char confirmation_prompt[256]; /* Generated voice prompt if confirmation needed */
    char normalized_summary[256];  /* Standard English & Pidgin accounting entry */
} NyxPidginTransaction;

/* Ledger Daily Session Aggregate */
typedef struct {
    double total_sales;
    double total_expenses;
    double total_credit_owed;
    double total_cash_collected;
    double total_transfer_collected;
    uint32_t transaction_count;
    char voice_readback_script[1024]; /* Voice output for "Read my ledger" */
} NyxPidginLedgerSummary;

/* Core Engine Lifecycle */
bool nyx_pidgin_init(void);
void nyx_pidgin_shutdown(void);

/* Parsing & Natural Language Understanding */
bool nyx_pidgin_parse_sentence(const char* input_text, NyxPidginTransaction* out_tx);
bool nyx_pidgin_parse_market_arithmetic(const char* text, double* out_qty, double* out_unit_price, double* out_total);
bool nyx_pidgin_normalize_currency(const char* token, double* out_amount);
bool nyx_pidgin_detect_unit(const char* token, NyxPidginUnit* out_unit, char* out_unit_name, size_t max_len);

/* Ledger Aggregator & Voice Synthesis */
bool nyx_pidgin_add_transaction_to_ledger(NyxPidginLedgerSummary* ledger, const NyxPidginTransaction* tx);
bool nyx_pidgin_generate_voice_readback(const NyxPidginLedgerSummary* ledger, char* out_buf, size_t max_len);

/* Morphological Tense/Aspect/Plural Tagging */
bool nyx_pidgin_is_completed_tense(const char* text); /* "don", "finish" */
bool nyx_pidgin_is_continuous_tense(const char* text); /* "dey" */
bool nyx_pidgin_is_future_tense(const char* text); /* "go", "wan" */
bool nyx_pidgin_is_plural_marker(const char* text); /* "dem" */

#ifdef __cplusplus
}
#endif

#endif /* NYX_PIDGIN_H */
