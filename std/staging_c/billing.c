#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "runtime/rt_arena.h"
#include "runtime/rt_string.h"
#include "runtime/rt_io.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_option.h"
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include "runtime/rt_net.h"
#include "runtime/rt_crypto.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_time.h"
#include "runtime/rt_nursery.h"
#include "runtime/rt_collections.h"
#include "runtime/rt_math.h"
#include "runtime/rt_format.h"
#include "runtime/rt_thread.h"
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
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

#define TAG_ProductType_Consumable 0
#define TAG_ProductType_NonConsumable 1
#define TAG_ProductType_AutoRenewingSubscription 2
#define TAG_ProductType_NonRenewingSubscription 3
typedef struct {
    int tag;
    union {
    } data;
} ProductType;

#define TAG_SubscriptionStatus_Active 0
#define TAG_SubscriptionStatus_InGracePeriod 1
#define TAG_SubscriptionStatus_InBillingRetry 2
#define TAG_SubscriptionStatus_Expired 3
#define TAG_SubscriptionStatus_Revoked 4
typedef struct {
    int tag;
    union {
    } data;
} SubscriptionStatus;

typedef struct {
    rt_string_t product_id;
    rt_string_t title;
    rt_string_t description;
    rt_string_t price_formatted;
    int64_t price_amount_micros;
    rt_string_t currency_code;
    ProductType product_type;
} ProductDetails;

typedef struct {
    rt_string_t transaction_id;
    rt_string_t original_transaction_id;
    rt_string_t product_id;
    int64_t purchase_date_epoch_ms;
    NyxOption expiration_date_epoch_ms;
    int is_verified_by_enclave;
    rt_string_t raw_jws_token;
} VerifiedTransaction;

typedef struct {
    int is_ready;
    NyxVec active_products;
} InAppBillingManager;

InAppBillingManager InAppBillingManager_new();
NyxResult InAppBillingManager_purchase(InAppBillingManager*, rt_string_t);
SubscriptionStatus InAppBillingManager_verify_receipt_in_enclave(InAppBillingManager*, VerifiedTransaction);
// module billing
// import std::string
// import std::vec
// import std::option
// import std::result
// import std::mobile::runtime
InAppBillingManager InAppBillingManager_new() {
NyxVec products = rt_vec_new(sizeof(int64_t));
rt_vec_push(&products, ({ ProductDetails _tmp_0 = (int64_t)((ProductDetails){.product_id = rt_string_from("com.nyx.pulsepay.pro_monthly"), .title = rt_string_from("PulsePay Enterprise Terminal (Monthly)"), .description = rt_string_from("Unlimited PQC Tap-to-Pay and contactless clearing"), .price_formatted = rt_string_from("$19.99"), .price_amount_micros = 19990000LL, .currency_code = rt_string_from("USD"), .product_type = (ProductType){ .tag = TAG_ProductType_AutoRenewingSubscription }}); &_tmp_0; }));
return (InAppBillingManager){.is_ready = 1, .active_products = products};
}

NyxResult InAppBillingManager_purchase(InAppBillingManager* self, rt_string_t product_id) {
NyxResult cap = require((MobileCapability){ .tag = TAG_MobileCapability_Billing });
({ if (({ NyxResult _r = cap; _r.tag == RESULT_ERR; })) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({{ rt_string_t _stmp_ = rt_string_from("InAppBilling unavailable: native billing runtime not linked"); _stmp_.data; }); _tmp; });
}
});
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({{ rt_string_t _stmp_ = rt_string_from("Purchase flow not implemented until billing runtime is linked"); _stmp_.data; }); _tmp; });
}

SubscriptionStatus InAppBillingManager_verify_receipt_in_enclave(InAppBillingManager* self, VerifiedTransaction transaction) {
return (SubscriptionStatus){ .tag = TAG_SubscriptionStatus_Revoked };
}

