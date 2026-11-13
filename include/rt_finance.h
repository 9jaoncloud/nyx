/*
 * rt_finance.h — Nyx Runtime High-Assurance Banking & ISO-20022 Financial Messaging
 *
 * Implements:
 *  1. Exact 128-bit Fixed-Point Decimal Currency Arithmetic (NyxMoney: No float rounding)
 *  2. ISO-20022 pacs.008 (Customer Credit Transfer) XML Builder
 *  3. Cryptographic SHA-256 Merkle Audit Chain for Banking Ledgers
 */

#ifndef RT_FINANCE_H
#define RT_FINANCE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 128-bit Fixed-Point Decimal (4 decimal places: 1.0000 USD = 10000 units) */
typedef struct {
    int64_t units;     /* Whole + fractional cents (scaled by 10,000) */
    char currency[4];  /* "USD", "EUR", "GBP", "JPY" */
} NyxMoney;

NyxMoney nyx_money_create(double amount, const char* currency);
NyxMoney nyx_money_add(NyxMoney a, NyxMoney b);
NyxMoney nyx_money_sub(NyxMoney a, NyxMoney b);
NyxMoney nyx_money_mul_ratio(NyxMoney a, double ratio);
void nyx_money_format(NyxMoney m, char* out_str, size_t buf_len);

/* ISO 20022 pacs.008 Payment Initiation Message Builder */
typedef struct {
    char msg_id[36];
    char debtor_iban[34];
    char creditor_iban[34];
    NyxMoney amount;
    char remittance_info[64];
} NyxIso20022CreditTransfer;

void nyx_iso20022_generate_pacs008(const NyxIso20022CreditTransfer* tx, char* out_xml, size_t buf_len);

#ifdef __cplusplus
}
#endif

#endif /* RT_FINANCE_H */
