#include "runtime/rt_http.h"
#include <math.h>
#include "runtime/rt_vec.h"
#include <stdio.h>
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include "runtime/rt_string.h"
#include "runtime/rt_map.h"
#include "runtime/rt_concurrency.h"
#include <string.h>
#include <stdlib.h>
#include "runtime/rt_option.h"
#include <stdint.h>
#include "runtime/rt_io.h"
#include "runtime/rt_db.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_arena.h"
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

