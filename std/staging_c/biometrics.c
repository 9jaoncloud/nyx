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

#define TAG_SecurityLevel_Class3Strong 0
#define TAG_SecurityLevel_Class2Weak 1
#define TAG_SecurityLevel_DeviceCredentialOnly 2
typedef struct {
    int tag;
    union {
    } data;
} SecurityLevel;

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
BiometricStatus BiometricManager_check_status(BiometricManager*);
NyxResult BiometricManager_authenticate_and_release_key(BiometricManager*, BiometricPromptConfig, rt_string_t);
DeviceAttestationReport BiometricManager_verify_device_integrity(BiometricManager*);
// module biometrics
// import std::string
// import std::vec
// import std::option
// import std::result
// import std::mobile::runtime
BiometricManager BiometricManager_new() {
return (BiometricManager){.active_modality = (BiometricModality){ .tag = TAG_BiometricModality_None }, .is_enrolled = 0};
}

BiometricStatus BiometricManager_check_status(BiometricManager* self) {
NyxResult cap = require((MobileCapability){ .tag = TAG_MobileCapability_Biometrics });
({ if (({ NyxResult _r = cap; _r.tag == RESULT_ERR; })) {
return (BiometricStatus){.is_available = 0, .modality = (*self).active_modality, .security_level = (SecurityLevel){ .tag = TAG_SecurityLevel_DeviceCredentialOnly }, .is_hardware_enclave_bound = 0};
}
});
return (BiometricStatus){.is_available = 1, .modality = (*self).active_modality, .security_level = (SecurityLevel){ .tag = TAG_SecurityLevel_Class3Strong }, .is_hardware_enclave_bound = 1};
}

NyxResult BiometricManager_authenticate_and_release_key(BiometricManager* self, BiometricPromptConfig config, rt_string_t key_alias) {
NyxResult cap = require((MobileCapability){ .tag = TAG_MobileCapability_Biometrics });
({ if (({ NyxResult _r = cap; _r.tag == RESULT_ERR; })) {
return (std.result.Result){ .tag = TAG_std.result.Result_Err, .data.Err = {.f0 = rt_string_from("Biometrics unavailable: native biometrics runtime not linked")} };
}
});
return (std.result.Result){ .tag = TAG_std.result.Result_Err, .data.Err = {.f0 = rt_string_from("Authentication flow not implemented until biometrics runtime is linked")} };
}

DeviceAttestationReport BiometricManager_verify_device_integrity(BiometricManager* self) {
return (DeviceAttestationReport){.verdict = (IntegrityVerdict){ .tag = TAG_IntegrityVerdict_FailedJailbreakRoot }, .is_emulator = 1, .has_debugger_attached = 1, .hardware_tpm_pcr_quote = rt_string_from(""), .timestamp_ms = 0LL};
}

