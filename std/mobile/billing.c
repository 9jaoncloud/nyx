#include "runtime/rt_io.h"
#include "runtime/rt_async.h"
#include "runtime/rt_map.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_json.h"
#include "runtime/rt_vec.h"
#include <stdint.h>
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include <stdlib.h>
#include <string.h>
#include "runtime/rt_option.h"
#include <stdio.h>
#include "runtime/rt_string.h"
#include <math.h>
#include "runtime/rt_arena.h"
#include "runtime/rt_stdlib.h"
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

#define TAG_MobileCapability_Display 0
#define TAG_MobileCapability_Sensors 1
#define TAG_MobileCapability_Battery 2
#define TAG_MobileCapability_Haptics 3
#define TAG_MobileCapability_Camera 4
#define TAG_MobileCapability_Bluetooth 5
#define TAG_MobileCapability_Nfc 6
#define TAG_MobileCapability_Biometrics 7
#define TAG_MobileCapability_Billing 8
#define TAG_MobileCapability_Push 9
#define TAG_MobileCapability_Audio 10
#define TAG_MobileCapability_FileSystem 11
typedef struct {
    int tag;
    union {
    } data;
} MobileCapability;

#define TAG_MobileError_RuntimeUnavailable 0
#define TAG_MobileError_CapabilityUnsupported 1
#define TAG_MobileError_NotInitialized 2
#define TAG_MobileError_DeviceError 3
#define TAG_MobileError_PermissionDenied 4
#define TAG_MobileError_UserCancelled 5
typedef struct {
    int tag;
    union {
    } data;
} MobileError;

#define TAG_MobilePlatform_Android 0
#define TAG_MobilePlatform_Ios 1
#define TAG_MobilePlatform_Simulator 2
#define TAG_MobilePlatform_Desktop 3
#define TAG_MobilePlatform_Unknown 4
typedef struct {
    int tag;
    union {
    } data;
} MobilePlatform;

typedef struct {
    int available;
    rt_string_t version;
    MobilePlatform platform;
    NyxVec capabilities;
} RuntimeProbe;

RuntimeProbe probe();
int is_available();
NyxResult require(MobileCapability);
// module runtime
// import std::string
// import std::vec
// import std::result
RuntimeProbe probe() {
return (RuntimeProbe){.available = 0, .version = rt_string_from("0.0.0-unlinked"), .platform = (MobilePlatform){ .tag = TAG_MobilePlatform_Unknown }, .capabilities = rt_vec_new(sizeof(int64_t))};
}

int is_available() {
return (probe()).available;
}

NyxResult require(MobileCapability cap) {
RuntimeProbe p = probe();
({ if ((!p.available)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ MobileError _stmp_ = (MobileError){ .tag = TAG_MobileError_RuntimeUnavailable }; &_stmp_; }); _tmp; });
}
});
int present = 0;
for (int64_t _c = 0; _c < ((NyxVec*)((&(p.capabilities))))->len; _c++) {
    MobileCapability c = *(MobileCapability*)rt_vec_get(*((NyxVec*)((&(p.capabilities)))), _c);
({ if ((c.tag == cap.tag)) {
present = 1;
}
});
}
;
({ if ((!present)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ MobileError _stmp_ = (MobileError){ .tag = TAG_MobileError_CapabilityUnsupported }; &_stmp_; }); _tmp; });
}
});
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)({  }); _tmp; });
}

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

#define TAG_ProductType_Consumable 0
#define TAG_ProductType_NonConsumable 1
#define TAG_ProductType_AutoRenewingSubscription 2
#define TAG_ProductType_NonRenewingSubscription 3
typedef struct {
    int tag;
    union {
    } data;
} ProductType;

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
NyxResult InAppBillingManager_purchase(void*, rt_string_t);
SubscriptionStatus InAppBillingManager_verify_receipt_in_enclave(void*, VerifiedTransaction);
// module billing
// import std::string
// import std::vec
// import std::option
// import std::result
// import std::mobile::runtime
InAppBillingManager InAppBillingManager_new() {
NyxVec products = rt_vec_new(sizeof(int64_t));
rt_vec_push(&products, ({ ProductDetails _tmp_0 = (ProductDetails){.product_id = rt_string_from("com.nyx.pulsepay.pro_monthly"), .title = rt_string_from("PulsePay Enterprise Terminal (Monthly)"), .description = rt_string_from("Unlimited PQC Tap-to-Pay and contactless clearing"), .price_formatted = rt_string_from("$19.99"), .price_amount_micros = 19990000LL, .currency_code = rt_string_from("USD"), .product_type = (ProductType){ .tag = TAG_ProductType_AutoRenewingSubscription }}; &_tmp_0; }));
return (InAppBillingManager){.is_ready = 1, .active_products = products};
}

NyxResult InAppBillingManager_purchase(void* self, rt_string_t product_id) {
NyxResult cap = require((MobileCapability){ .tag = TAG_MobileCapability_Billing });
({ if (({ NyxResult _r = cap; _r.tag == RESULT_ERR; })) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({{ rt_string_t _stmp_ = rt_string_from("InAppBilling unavailable: native billing runtime not linked"); _stmp_.data; }); _tmp; });
}
});
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({{ rt_string_t _stmp_ = rt_string_from("Purchase flow not implemented until billing runtime is linked"); _stmp_.data; }); _tmp; });
}

SubscriptionStatus InAppBillingManager_verify_receipt_in_enclave(void* self, VerifiedTransaction transaction) {
return (SubscriptionStatus){ .tag = TAG_SubscriptionStatus_Revoked };
}

