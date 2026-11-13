/*
 * rt_finance.c — Nyx Runtime Banking & ISO-20022 Financial Messaging Implementation
 */

#include "rt_finance.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCALE_FACTOR 10000LL

NyxMoney nyx_money_create(double amount, const char* currency) {
    NyxMoney m;
    m.units = (int64_t)(amount * SCALE_FACTOR + (amount >= 0 ? 0.5 : -0.5));
    if (currency) strncpy(m.currency, currency, 3);
    m.currency[3] = '\0';
    return m;
}

NyxMoney nyx_money_add(NyxMoney a, NyxMoney b) {
    NyxMoney res = a;
    res.units = a.units + b.units;
    return res;
}

NyxMoney nyx_money_sub(NyxMoney a, NyxMoney b) {
    NyxMoney res = a;
    res.units = a.units - b.units;
    return res;
}

NyxMoney nyx_money_mul_ratio(NyxMoney a, double ratio) {
    NyxMoney res = a;
    res.units = (int64_t)((double)a.units * ratio + 0.5);
    return res;
}

void nyx_money_format(NyxMoney m, char* out_str, size_t buf_len) {
    if (!out_str || buf_len == 0) return;
    int64_t whole = m.units / SCALE_FACTOR;
    int64_t frac = llabs(m.units % SCALE_FACTOR);
    snprintf(out_str, buf_len, "%s %lld.%04lld", m.currency, (long long)whole, (long long)frac);
}

void nyx_iso20022_generate_pacs008(const NyxIso20022CreditTransfer* tx, char* out_xml, size_t buf_len) {
    if (!tx || !out_xml || buf_len == 0) return;

    double amount_val = (double)tx->amount.units / SCALE_FACTOR;

    snprintf(out_xml, buf_len,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<Document xmlns=\"urn:iso:std:iso:20022:tech:xsd:pacs.008.001.08\">\n"
        "  <FIToFICstmrCdtTrf>\n"
        "    <GrpHdr>\n"
        "      <MsgId>%s</MsgId>\n"
        "      <CreDtTm>2026-08-29T06:30:00Z</CreDtTm>\n"
        "      <NbOfTxs>1</NbOfTxs>\n"
        "    </GrpHdr>\n"
        "    <CdtTrfTxInf>\n"
        "      <IntrBkSttlmAmt Ccy=\"%s\">%.2f</IntrBkSttlmAmt>\n"
        "      <DbtrAcct><Id><IBAN>%s</IBAN></Id></DbtrAcct>\n"
        "      <CdtrAcct><Id><IBAN>%s</IBAN></Id></CdtrAcct>\n"
        "      <RmtInf><Ustrd>%s</Ustrd></RmtInf>\n"
        "    </CdtTrfTxInf>\n"
        "  </FIToFICstmrCdtTrf>\n"
        "</Document>",
        tx->msg_id, tx->amount.currency, amount_val,
        tx->debtor_iban, tx->creditor_iban, tx->remittance_info);
}
