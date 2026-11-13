#include "runtime/rt_concurrency.h"
#include "runtime/rt_option.h"
#include "runtime/rt_db.h"
#include <stdint.h>
#include "runtime/rt_async.h"
#include "runtime/rt_vec.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "runtime/rt_string.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_http.h"
#include "runtime/rt_io.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_map.h"
#include <string.h>
#include "runtime/rt_json.h"
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

#define TAG_IntegrityVerdict_MeetsStrongIntegrity 0
#define TAG_IntegrityVerdict_MeetsDeviceIntegrity 1
#define TAG_IntegrityVerdict_FailedJailbreakRoot 2
typedef struct {
    int tag;
    union {
    } data;
} IntegrityVerdict;

#define TAG_BiometricModality_FaceId 0
#define TAG_BiometricModality_Fingerprint 1
#define TAG_BiometricModality_Iris 2
#define TAG_BiometricModality_None 3
typedef struct {
    int tag;
    union {
    } data;
} BiometricModality;

#define TAG_SecurityLevel_Class3Strong 0
#define TAG_SecurityLevel_Class2Weak 1
#define TAG_SecurityLevel_DeviceCredentialOnly 2
typedef struct {
    int tag;
    union {
    } data;
} SecurityLevel;

typedef struct {
    int is_available;
    BiometricModality modality;
    SecurityLevel security_level;
    int is_hardware_enclave_bound;
} BiometricStatus;

typedef struct {
    rt_string_t title;
    rt_string_t subtitle;
    rt_string_t cancel_button_label;
    int allow_fallback_to_pin;
} BiometricPromptConfig;

typedef struct {
    int is_successful;
    BiometricModality modality_used;
    NyxOption released_enclave_key_handle;
    rt_string_t hardware_signature_jwt;
} BiometricAuthResult;

typedef struct {
    IntegrityVerdict verdict;
    int is_emulator;
    int has_debugger_attached;
    rt_string_t hardware_tpm_pcr_quote;
    int64_t timestamp_ms;
} DeviceAttestationReport;

typedef struct {
    BiometricModality active_modality;
    int is_enrolled;
} BiometricManager;

BiometricManager BiometricManager_new();
BiometricStatus BiometricManager_check_status(void*);
NyxResult BiometricManager_authenticate_and_release_key(void*, BiometricPromptConfig, rt_string_t);
DeviceAttestationReport BiometricManager_verify_device_integrity(void*);
// module biometrics
// import std::string
// import std::vec
// import std::option
// import std::result
// import std::mobile::runtime
BiometricManager BiometricManager_new() {
return (BiometricManager){.active_modality = (BiometricModality){ .tag = TAG_BiometricModality_None }, .is_enrolled = 0};
}

BiometricStatus BiometricManager_check_status(void* self) {
NyxResult cap = require((MobileCapability){ .tag = TAG_MobileCapability_Biometrics });
({ if (({ NyxResult _r = cap; _r.tag == RESULT_ERR; })) {
return (BiometricStatus){.is_available = 0, .modality = (*self).active_modality, .security_level = (SecurityLevel){ .tag = TAG_SecurityLevel_DeviceCredentialOnly }, .is_hardware_enclave_bound = 0};
}
});
return (BiometricStatus){.is_available = 1, .modality = (*self).active_modality, .security_level = (SecurityLevel){ .tag = TAG_SecurityLevel_Class3Strong }, .is_hardware_enclave_bound = 1};
}

NyxResult BiometricManager_authenticate_and_release_key(void* self, BiometricPromptConfig config, rt_string_t key_alias) {
NyxResult cap = require((MobileCapability){ .tag = TAG_MobileCapability_Biometrics });
({ if (({ NyxResult _r = cap; _r.tag == RESULT_ERR; })) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({{ rt_string_t _stmp_ = rt_string_from("Biometrics unavailable: native biometrics runtime not linked"); _stmp_.data; }); _tmp; });
}
});
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({{ rt_string_t _stmp_ = rt_string_from("Authentication flow not implemented until biometrics runtime is linked"); _stmp_.data; }); _tmp; });
}

DeviceAttestationReport BiometricManager_verify_device_integrity(void* self) {
return (DeviceAttestationReport){.verdict = (IntegrityVerdict){ .tag = TAG_IntegrityVerdict_FailedJailbreakRoot }, .is_emulator = 1, .has_debugger_attached = 1, .hardware_tpm_pcr_quote = rt_string_from(""), .timestamp_ms = 0LL};
}

