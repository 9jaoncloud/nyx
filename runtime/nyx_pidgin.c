/**
 * @file nyx_pidgin.c
 * @brief Nigerian Pidgin (Naija) Natural Language & Sovereign Market Intelligence Engine
 * @author Simeon Bala (9jaonCloud Engineering & Sovereign Computing Initiative)
 */

#include "nyx_pidgin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* Helper: lowercases a string into target buffer */
static void str_tolower(const char* src, char* dst, size_t dst_size) {
    if (!src || !dst || dst_size == 0) return;
    size_t i = 0;
    while (src[i] && i + 1 < dst_size) {
        dst[i] = (char)tolower((unsigned char)src[i]);
        i++;
    }
    dst[i] = '\0';
}

/* Helper: trim whitespace */
static char* str_trim(char* s) {
    if (!s) return NULL;
    while (isspace((unsigned char)*s)) s++;
    if (*s == 0) return s;
    char* back = s + strlen(s) - 1;
    while (back > s && isspace((unsigned char)*back)) back--;
    *(back + 1) = '\0';
    return s;
}

/* Helper: check if haystack contains whole word */
static bool contains_word(const char* haystack, const char* word) {
    if (!haystack || !word) return false;
    const char* ptr = haystack;
    size_t wlen = strlen(word);
    while ((ptr = strstr(ptr, word)) != NULL) {
        bool left_ok = (ptr == haystack || isspace((unsigned char)*(ptr - 1)) || ispunct((unsigned char)*(ptr - 1)));
        bool right_ok = (ptr[wlen] == '\0' || isspace((unsigned char)ptr[wlen]) || ispunct((unsigned char)ptr[wlen]));
        if (left_ok && right_ok) return true;
        ptr += 1;
    }
    return false;
}

bool nyx_pidgin_init(void) {
    return true;
}

void nyx_pidgin_shutdown(void) {
    /* No-op */
}

bool nyx_pidgin_is_completed_tense(const char* text) {
    if (!text) return false;
    char lower[512];
    str_tolower(text, lower, sizeof(lower));
    return contains_word(lower, "don") || contains_word(lower, "finish") || 
           contains_word(lower, "done") || contains_word(lower, "ti");
}

bool nyx_pidgin_is_continuous_tense(const char* text) {
    if (!text) return false;
    char lower[512];
    str_tolower(text, lower, sizeof(lower));
    return contains_word(lower, "dey") || contains_word(lower, "na");
}

bool nyx_pidgin_is_future_tense(const char* text) {
    if (!text) return false;
    char lower[512];
    str_tolower(text, lower, sizeof(lower));
    return contains_word(lower, "go") || contains_word(lower, "wan") || contains_word(lower, "will");
}

bool nyx_pidgin_is_plural_marker(const char* text) {
    if (!text) return false;
    char lower[512];
    str_tolower(text, lower, sizeof(lower));
    return contains_word(lower, "dem");
}

/* Detect Dialect / Code-Switching */
static NyxPidginDialect detect_dialect(const char* text_lower) {
    /* Yoruba markers */
    if (strstr(text_lower, "ba mi") || strstr(text_lower, "owo") || strstr(text_lower, "elo ni") ||
        strstr(text_lower, "gbese") || strstr(text_lower, "kosi") || strstr(text_lower, "rara") ||
        strstr(text_lower, "sebi") || strstr(text_lower, "abi")) {
        return PIDGIN_DIALECT_YORUBA_MIXED;
    }
    /* Hausa markers */
    if (strstr(text_lower, "kudi") || strstr(text_lower, "nawa ne") || strstr(text_lower, "sannu") ||
        strstr(text_lower, "ba komai") || strstr(text_lower, "yauwa") || strstr(text_lower, "ina son")) {
        return PIDGIN_DIALECT_HAUSA_MIXED;
    }
    /* Igbo markers */
    if (strstr(text_lower, "ego") || strstr(text_lower, "kedu") || strstr(text_lower, "biko") ||
        strstr(text_lower, "nna") || strstr(text_lower, "otu") || strstr(text_lower, "dalunu")) {
        return PIDGIN_DIALECT_IGBO_MIXED;
    }
    return PIDGIN_DIALECT_STANDARD_NAIJA;
}

/* Unit Detection */
bool nyx_pidgin_detect_unit(const char* token, NyxPidginUnit* out_unit, char* out_unit_name, size_t max_len) {
    if (!token || !out_unit) return false;
    char lower[64];
    str_tolower(token, lower, sizeof(lower));

    if (strcmp(lower, "mudu") == 0 || strcmp(lower, "mudus") == 0) {
        *out_unit = PIDGIN_UNIT_MUDU;
        if (out_unit_name) snprintf(out_unit_name, max_len, "mudu");
        return true;
    }
    if (strcmp(lower, "derica") == 0 || strcmp(lower, "dericas") == 0 || strcmp(lower, "derika") == 0 || strcmp(lower, "delica") == 0) {
        *out_unit = PIDGIN_UNIT_DERICA;
        if (out_unit_name) snprintf(out_unit_name, max_len, "derica");
        return true;
    }
    if (strcmp(lower, "painter") == 0 || strcmp(lower, "painters") == 0 || strcmp(lower, "paint") == 0) {
        *out_unit = PIDGIN_UNIT_PAINTER;
        if (out_unit_name) snprintf(out_unit_name, max_len, "painter");
        return true;
    }
    if (strcmp(lower, "kongo") == 0 || strcmp(lower, "kongos") == 0) {
        *out_unit = PIDGIN_UNIT_KONGO;
        if (out_unit_name) snprintf(out_unit_name, max_len, "kongo");
        return true;
    }
    if (strcmp(lower, "half bag") == 0 || strcmp(lower, "half-bag") == 0) {
        *out_unit = PIDGIN_UNIT_HALF_BAG;
        if (out_unit_name) snprintf(out_unit_name, max_len, "half bag");
        return true;
    }
    if (strcmp(lower, "bag") == 0 || strcmp(lower, "bags") == 0) {
        *out_unit = PIDGIN_UNIT_BAG;
        if (out_unit_name) snprintf(out_unit_name, max_len, "bag");
        return true;
    }
    if (strcmp(lower, "tin") == 0 || strcmp(lower, "tins") == 0 || strcmp(lower, "cup") == 0 || strcmp(lower, "cups") == 0) {
        *out_unit = PIDGIN_UNIT_TIN;
        if (out_unit_name) snprintf(out_unit_name, max_len, "tin");
        return true;
    }
    if (strcmp(lower, "carton") == 0 || strcmp(lower, "cartons") == 0 || strcmp(lower, "kato") == 0 || strcmp(lower, "pack") == 0 || strcmp(lower, "packs") == 0) {
        *out_unit = PIDGIN_UNIT_CARTON;
        if (out_unit_name) snprintf(out_unit_name, max_len, "carton");
        return true;
    }
    if (strcmp(lower, "crate") == 0 || strcmp(lower, "crates") == 0) {
        *out_unit = PIDGIN_UNIT_CRATE;
        if (out_unit_name) snprintf(out_unit_name, max_len, "crate");
        return true;
    }
    if (strcmp(lower, "rubber") == 0 || strcmp(lower, "rubbers") == 0) {
        *out_unit = PIDGIN_UNIT_RUBBER;
        if (out_unit_name) snprintf(out_unit_name, max_len, "rubber");
        return true;
    }
    if (strcmp(lower, "tie") == 0 || strcmp(lower, "ties") == 0 || strcmp(lower, "bundle") == 0 || strcmp(lower, "bundles") == 0) {
        *out_unit = PIDGIN_UNIT_TIE;
        if (out_unit_name) snprintf(out_unit_name, max_len, "tie");
        return true;
    }
    if (strcmp(lower, "tuber") == 0 || strcmp(lower, "tubers") == 0 || strcmp(lower, "head") == 0 || strcmp(lower, "heads") == 0) {
        *out_unit = PIDGIN_UNIT_TUBER;
        if (out_unit_name) snprintf(out_unit_name, max_len, "tuber");
        return true;
    }
    if (strcmp(lower, "piece") == 0 || strcmp(lower, "pieces") == 0 || strcmp(lower, "pcs") == 0) {
        *out_unit = PIDGIN_UNIT_PIECE;
        if (out_unit_name) snprintf(out_unit_name, max_len, "piece");
        return true;
    }

    return false;
}

/* Parse Currency token, e.g. "5k" -> 5000, "1.5m" -> 1500000, "1500", "#2000", "2,500" */
bool nyx_pidgin_normalize_currency(const char* token, double* out_amount) {
    if (!token || !out_amount) return false;
    char clean[64];
    size_t ci = 0;
    bool is_k = false;
    bool is_m = false;

    for (size_t i = 0; token[i] && ci + 1 < sizeof(clean); i++) {
        char c = token[i];
        if (isdigit((unsigned char)c) || c == '.') {
            clean[ci++] = c;
        } else if (c == 'k' || c == 'K') {
            is_k = true;
        } else if (c == 'm' || c == 'M') {
            is_m = true;
        } else if (c == ',' || c == '#' || c == 'N' || c == 'n') {
            /* Skip currency symbols/commas */
            continue;
        }
    }
    clean[ci] = '\0';
    if (ci == 0) return false;

    char* endptr = NULL;
    double val = strtod(clean, &endptr);
    if (endptr == clean) return false;

    if (is_k) val *= 1000.0;
    if (is_m) val *= 1000000.0;

    *out_amount = val;
    return true;
}

/* Parse numbers expressed in words or digits */
static bool parse_number_token(const char* token, double* out_num) {
    if (!token || !out_num) return false;
    char lower[64];
    str_tolower(token, lower, sizeof(lower));

    if (strcmp(lower, "one") == 0 || strcmp(lower, "1") == 0) { *out_num = 1.0; return true; }
    if (strcmp(lower, "two") == 0 || strcmp(lower, "2") == 0) { *out_num = 2.0; return true; }
    if (strcmp(lower, "three") == 0 || strcmp(lower, "3") == 0) { *out_num = 3.0; return true; }
    if (strcmp(lower, "four") == 0 || strcmp(lower, "4") == 0) { *out_num = 4.0; return true; }
    if (strcmp(lower, "five") == 0 || strcmp(lower, "5") == 0) { *out_num = 5.0; return true; }
    if (strcmp(lower, "six") == 0 || strcmp(lower, "6") == 0) { *out_num = 6.0; return true; }
    if (strcmp(lower, "seven") == 0 || strcmp(lower, "7") == 0) { *out_num = 7.0; return true; }
    if (strcmp(lower, "eight") == 0 || strcmp(lower, "8") == 0) { *out_num = 8.0; return true; }
    if (strcmp(lower, "nine") == 0 || strcmp(lower, "9") == 0) { *out_num = 9.0; return true; }
    if (strcmp(lower, "ten") == 0 || strcmp(lower, "10") == 0) { *out_num = 10.0; return true; }
    if (strcmp(lower, "half") == 0 || strcmp(lower, "0.5") == 0) { *out_num = 0.5; return true; }

    return nyx_pidgin_normalize_currency(token, out_num);
}

/* Detect Repeated Unit Price Multiplier Idioms:
 * e.g. "two to 50" -> 250
 * e.g. "two two fifty" -> 250
 * e.g. "two two hundred" -> 200
 * e.g. "three three hundred" -> 300
 * e.g. "five five hundred" -> 500
 * e.g. "one one fifty" -> 150
 */
static bool detect_repeated_unit_price(const char* text_lower, double* out_unit_price) {
    if (!text_lower || !out_unit_price) return false;

    if (strstr(text_lower, "two to 50") || strstr(text_lower, "two to fifty") || strstr(text_lower, "two two fifty") || strstr(text_lower, "2 2 50")) {
        *out_unit_price = 250.0;
        return true;
    }
    if (strstr(text_lower, "two two hundred") || strstr(text_lower, "two to hundred") || strstr(text_lower, "2 2 100")) {
        *out_unit_price = 200.0;
        return true;
    }
    if (strstr(text_lower, "three three hundred") || strstr(text_lower, "three to hundred") || strstr(text_lower, "3 3 100")) {
        *out_unit_price = 300.0;
        return true;
    }
    if (strstr(text_lower, "four four hundred") || strstr(text_lower, "4 4 100")) {
        *out_unit_price = 400.0;
        return true;
    }
    if (strstr(text_lower, "five five hundred") || strstr(text_lower, "5 5 100") || strstr(text_lower, "five to hundred")) {
        *out_unit_price = 500.0;
        return true;
    }
    if (strstr(text_lower, "one one fifty") || strstr(text_lower, "one to fifty") || strstr(text_lower, "1 1 50")) {
        *out_unit_price = 150.0;
        return true;
    }
    if (strstr(text_lower, "one one hundred") || strstr(text_lower, "one to hundred") || strstr(text_lower, "1 1 100")) {
        *out_unit_price = 100.0;
        return true;
    }
    if (strstr(text_lower, "fifty fifty") || strstr(text_lower, "50 50")) {
        *out_unit_price = 50.0;
        return true;
    }
    if (strstr(text_lower, "one one thousand") || strstr(text_lower, "1 1 1000")) {
        *out_unit_price = 1000.0;
        return true;
    }
    return false;
}

/* Parse Nigerian Market Arithmetic */
bool nyx_pidgin_parse_market_arithmetic(const char* text, double* out_qty, double* out_unit_price, double* out_total) {
    if (!text) return false;
    char lower[512];
    str_tolower(text, lower, sizeof(lower));

    double qty = 1.0;
    double unit_p = 0.0;
    double total = 0.0;

    /* Check repeated unit price first */
    bool has_repeated_price = detect_repeated_unit_price(lower, &unit_p);

    /* Tokenize into words */
    char temp[512];
    strncpy(temp, lower, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    char* words[32];
    int wcount = 0;
    char* tok = strtok(temp, " ,;:");
    while (tok && wcount < 32) {
        words[wcount++] = tok;
        tok = strtok(NULL, " ,;:");
    }

    bool qty_found = false;
    bool total_found = false;

    for (int i = 0; i < wcount; i++) {
        double num = 0.0;
        if (parse_number_token(words[i], &num)) {
            if (!qty_found && num <= 100.0 && (i + 1 < wcount)) {
                /* If followed by a unit or item name, treat as quantity */
                NyxPidginUnit u;
                char uname[32];
                if (nyx_pidgin_detect_unit(words[i+1], &u, uname, sizeof(uname)) ||
                    strcmp(words[i+1], "tins") == 0 || strcmp(words[i+1], "tin") == 0 ||
                    strcmp(words[i+1], "mudu") == 0 || strcmp(words[i+1], "bag") == 0 ||
                    strcmp(words[i+1], "rice") == 0 || strcmp(words[i+1], "garri") == 0 ||
                    strcmp(words[i+1], "beans") == 0 || strcmp(words[i+1], "yam") == 0 ||
                    strcmp(words[i+1], "oil") == 0 || strcmp(words[i+1], "indomie") == 0) {
                    qty = num;
                    qty_found = true;
                    continue;
                }
            }

            if (!has_repeated_price && num >= 100.0) {
                total = num;
                total_found = true;
            } else if (has_repeated_price && num >= 100.0 && fabs(num - unit_p) > 0.01) {
                total = num;
                total_found = true;
            }
        }
    }

    if (has_repeated_price) {
        if (total == 0.0 && qty > 0.0) {
            total = qty * unit_p;
        }
    } else if (total_found && qty > 0.0 && unit_p == 0.0) {
        unit_p = total / qty;
    }

    if (out_qty) *out_qty = qty;
    if (out_unit_price) *out_unit_price = unit_p;
    if (out_total) *out_total = total;
    return (total > 0.0 || unit_p > 0.0);
}

/* Parse sentence into structured NyxPidginTransaction */
bool nyx_pidgin_parse_sentence(const char* input_text, NyxPidginTransaction* out_tx) {
    if (!input_text || !out_tx) return false;
    memset(out_tx, 0, sizeof(NyxPidginTransaction));

    char lower[512];
    str_tolower(input_text, lower, sizeof(lower));
    out_tx->dialect = detect_dialect(lower);
    out_tx->confidence = 0.90;
    out_tx->payment_mode = PIDGIN_PAYMENT_CASH;

    /* Intent Classification */
    if (contains_word(lower, "yes") || contains_word(lower, "na so") || contains_word(lower, "correct") ||
        contains_word(lower, "ehen") || contains_word(lower, "confirm") || contains_word(lower, "sure")) {
        out_tx->intent = PIDGIN_INTENT_CONFIRMATION_YES;
        snprintf(out_tx->normalized_summary, sizeof(out_tx->normalized_summary), "User Confirmed: Yes");
        return true;
    }

    if (contains_word(lower, "no") || contains_word(lower, "rara") || contains_word(lower, "e no correct") ||
        contains_word(lower, "cancel") || contains_word(lower, "wrong")) {
        out_tx->intent = PIDGIN_INTENT_CONFIRMATION_NO;
        snprintf(out_tx->normalized_summary, sizeof(out_tx->normalized_summary), "User Cancelled: No");
        return true;
    }

    if (strstr(lower, "read my ledger") || strstr(lower, "read ledger") || strstr(lower, "wetin be my total") ||
        strstr(lower, "how much i make today") || strstr(lower, "today sales")) {
        out_tx->intent = PIDGIN_INTENT_QUERY_SUMMARY;
        snprintf(out_tx->normalized_summary, sizeof(out_tx->normalized_summary), "Query: Daily Ledger Summary");
        return true;
    }

    if (strstr(lower, "who dey owe") || strstr(lower, "who owe me") || strstr(lower, "list debtors") ||
        strstr(lower, "check gbese")) {
        out_tx->intent = PIDGIN_INTENT_QUERY_DEBTORS;
        snprintf(out_tx->normalized_summary, sizeof(out_tx->normalized_summary), "Query: Debtor Accounts");
        return true;
    }

    if (strstr(lower, "how many") || strstr(lower, "remain") || strstr(lower, "stock") || strstr(lower, "inventory")) {
        out_tx->intent = PIDGIN_INTENT_QUERY_INVENTORY;
        snprintf(out_tx->normalized_summary, sizeof(out_tx->normalized_summary), "Query: Inventory Level");
        return true;
    }

    /* Debt Repayment */
    if (strstr(lower, "pay in debt") || strstr(lower, "pay gbese") || strstr(lower, "pay balance") ||
        (contains_word(lower, "pay") && (contains_word(lower, "debt") || contains_word(lower, "owe") || contains_word(lower, "gbese")))) {
        out_tx->intent = PIDGIN_INTENT_DEBT_REPAYMENT;
    }
    /* Credit Sale / Debt given */
    else if (strstr(lower, "credit") || strstr(lower, "gbese") || strstr(lower, "owe") || strstr(lower, "collect on credit") || strstr(lower, "collect gbese")) {
        out_tx->intent = PIDGIN_INTENT_CREDIT_SALE;
        out_tx->payment_mode = PIDGIN_PAYMENT_CREDIT_UNPAID;
    }
    /* Purchase Expense */
    else if (contains_word(lower, "buy") || contains_word(lower, "purchase") || contains_word(lower, "depot") || contains_word(lower, "expense")) {
        out_tx->intent = PIDGIN_INTENT_PURCHASE_EXPENSE;
    }
    /* Sale Transaction */
    else {
        out_tx->intent = PIDGIN_INTENT_SALE;
    }

    /* Payment Mode Detection */
    if (strstr(lower, "transfer") || strstr(lower, "trans") || strstr(lower, "bank")) {
        out_tx->payment_mode = PIDGIN_PAYMENT_TRANSFER;
    } else if (strstr(lower, "cash")) {
        out_tx->payment_mode = PIDGIN_PAYMENT_CASH;
    }

    /* Extract Quantities, Unit Price, and Total */
    nyx_pidgin_parse_market_arithmetic(lower, &out_tx->quantity, &out_tx->unit_price, &out_tx->total_amount);

    /* Extract Common Items */
    const char* items[] = {
        "garri", "rice", "beans", "yam", "indomie", "palm oil", "groundnut oil", "oil",
        "sugar", "milk", "egg", "eggs", "bread", "fish", "meat", "maggi", "salt",
        "cement", "fuel", "kerosene", "recharge card", "flour", "plantain", "onions", "tomatoes",
        "spaghetti", "detergent", "soap"
    };
    for (size_t i = 0; i < sizeof(items)/sizeof(items[0]); i++) {
        if (contains_word(lower, items[i])) {
            snprintf(out_tx->item_name, sizeof(out_tx->item_name), "%s", items[i]);
            break;
        }
    }
    if (out_tx->item_name[0] == '\0') {
        snprintf(out_tx->item_name, sizeof(out_tx->item_name), "General Item");
    }

    /* Extract Units */
    const char* unit_tokens[] = {
        "mudu", "derica", "painter", "kongo", "half bag", "bag", "tin", "cup",
        "carton", "crate", "rubber", "tie", "tuber", "piece", "pack"
    };
    for (size_t i = 0; i < sizeof(unit_tokens)/sizeof(unit_tokens[0]); i++) {
        if (contains_word(lower, unit_tokens[i])) {
            nyx_pidgin_detect_unit(unit_tokens[i], &out_tx->unit, out_tx->unit_name, sizeof(out_tx->unit_name));
            break;
        }
    }
    if (out_tx->unit_name[0] == '\0') {
        out_tx->unit = PIDGIN_UNIT_ITEM;
        snprintf(out_tx->unit_name, sizeof(out_tx->unit_name), "item");
    }

    /* Extract Customer Name (e.g., Mama Chinedu, Baba Tunde, Emeka, Sister Blessing, Alhaji) */
    const char* names[] = {
        "mama chinedu", "baba tunde", "mama ngozi", "sister blessing", "brother emeka",
        "alhaji musa", "mrs adamu", "mr okafor", "iya basira", "mama bolu", "chidi",
        "emeka", "musa", "tunde", "ada", "blessing", "funke", "bolaji"
    };
    for (size_t i = 0; i < sizeof(names)/sizeof(names[0]); i++) {
        if (strstr(lower, names[i])) {
            /* Capitalize words */
            char cname[64];
            snprintf(cname, sizeof(cname), "%s", names[i]);
            cname[0] = (char)toupper((unsigned char)cname[0]);
            char* sp = strchr(cname, ' ');
            if (sp && sp[1]) sp[1] = (char)toupper((unsigned char)sp[1]);
            snprintf(out_tx->customer_name, sizeof(out_tx->customer_name), "%s", cname);
            break;
        }
    }

    /* Ambiguity & Confirmation Prompts */
    if (out_tx->total_amount <= 0.0) {
        out_tx->needs_confirmation = true;
        out_tx->confidence = 0.50;
        snprintf(out_tx->confirmation_prompt, sizeof(out_tx->confirmation_prompt),
                 "Biko/E joo, how much be the total money for the %s?", out_tx->item_name);
    } else if (out_tx->intent == PIDGIN_INTENT_CREDIT_SALE && out_tx->customer_name[0] == '\0') {
        out_tx->needs_confirmation = true;
        out_tx->confidence = 0.65;
        snprintf(out_tx->confirmation_prompt, sizeof(out_tx->confirmation_prompt),
                 "Who collect the %0.0f %s %s on credit?", out_tx->quantity, out_tx->unit_name, out_tx->item_name);
    } else {
        out_tx->needs_confirmation = false;
        out_tx->confidence = 0.95;
    }

    /* Generate Normalized Summary */
    if (out_tx->intent == PIDGIN_INTENT_SALE) {
        snprintf(out_tx->normalized_summary, sizeof(out_tx->normalized_summary),
                 "SALE: %0.0f %s of %s @ N%.0f = N%.0f (%s)",
                 out_tx->quantity, out_tx->unit_name, out_tx->item_name,
                 out_tx->unit_price, out_tx->total_amount,
                 out_tx->payment_mode == PIDGIN_PAYMENT_TRANSFER ? "Transfer" : "Cash");
    } else if (out_tx->intent == PIDGIN_INTENT_CREDIT_SALE) {
        snprintf(out_tx->normalized_summary, sizeof(out_tx->normalized_summary),
                 "CREDIT: %s took %0.0f %s of %s = N%.0f (Unpaid Gbese)",
                 out_tx->customer_name[0] ? out_tx->customer_name : "Customer",
                 out_tx->quantity, out_tx->unit_name, out_tx->item_name, out_tx->total_amount);
    } else if (out_tx->intent == PIDGIN_INTENT_PURCHASE_EXPENSE) {
        snprintf(out_tx->normalized_summary, sizeof(out_tx->normalized_summary),
                 "EXPENSE: Bought %0.0f %s of %s = N%.0f",
                 out_tx->quantity, out_tx->unit_name, out_tx->item_name, out_tx->total_amount);
    } else if (out_tx->intent == PIDGIN_INTENT_DEBT_REPAYMENT) {
        snprintf(out_tx->normalized_summary, sizeof(out_tx->normalized_summary),
                 "REPAYMENT: %s paid N%.0f debt",
                 out_tx->customer_name[0] ? out_tx->customer_name : "Customer",
                 out_tx->total_amount);
    }

    return true;
}

/* Ledger Aggregator */
bool nyx_pidgin_add_transaction_to_ledger(NyxPidginLedgerSummary* ledger, const NyxPidginTransaction* tx) {
    if (!ledger || !tx) return false;

    if (tx->intent == PIDGIN_INTENT_SALE) {
        ledger->total_sales += tx->total_amount;
        if (tx->payment_mode == PIDGIN_PAYMENT_TRANSFER) {
            ledger->total_transfer_collected += tx->total_amount;
        } else {
            ledger->total_cash_collected += tx->total_amount;
        }
        ledger->transaction_count++;
    } else if (tx->intent == PIDGIN_INTENT_CREDIT_SALE) {
        ledger->total_credit_owed += tx->total_amount;
        ledger->transaction_count++;
    } else if (tx->intent == PIDGIN_INTENT_PURCHASE_EXPENSE) {
        ledger->total_expenses += tx->total_amount;
        ledger->transaction_count++;
    } else if (tx->intent == PIDGIN_INTENT_DEBT_REPAYMENT) {
        ledger->total_credit_owed -= tx->total_amount;
        if (ledger->total_credit_owed < 0.0) ledger->total_credit_owed = 0.0;
        ledger->total_cash_collected += tx->total_amount;
        ledger->transaction_count++;
    }

    return true;
}

/* Generate Voice Readback Script in Authentic Nigerian Pidgin */
bool nyx_pidgin_generate_voice_readback(const NyxPidginLedgerSummary* ledger, char* out_buf, size_t max_len) {
    if (!ledger || !out_buf || max_len == 0) return false;

    snprintf(out_buf, max_len,
             "Oga Madam, today total sales na %0.0f naira across %u market transactions. "
             "Cash wey dey hand na %0.0f naira, bank transfer na %0.0f naira. "
             "Money wey people dey owe you na %0.0f naira, and expense wey you spend na %0.0f naira. "
             "Your market sharp today well well!",
             ledger->total_sales,
             (unsigned int)ledger->transaction_count,
             ledger->total_cash_collected,
             ledger->total_transfer_collected,
             ledger->total_credit_owed,
             ledger->total_expenses);
    return true;
}
