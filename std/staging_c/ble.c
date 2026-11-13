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

typedef struct {
    rt_string_t device_uuid;
    int64_t rssi_dbm;
    int64_t packet_len;
    int is_authenticated;
} BleDeviceFrame;

typedef struct {
    int64_t waveform_id;
    int64_t intensity_percent;
    int64_t duration_ms;
} HapticController;

BleDeviceFrame BleDeviceFrame_new(rt_string_t, int64_t);
int BleDeviceFrame_is_signal_strong(BleDeviceFrame*);
HapticController HapticController_new_sharp_click();
int HapticController_trigger_haptic(HapticController*);
// module std.mobile.ble
BleDeviceFrame BleDeviceFrame_new(rt_string_t uuid, int64_t rssi) {
return (BleDeviceFrame){.device_uuid = uuid, .rssi_dbm = rssi, .packet_len = 31LL, .is_authenticated = 1};
}

int BleDeviceFrame_is_signal_strong(BleDeviceFrame* self) {
if (((*self).rssi_dbm >= (-75LL))) {
return 1;
} else {
return 0;
}
}

HapticController HapticController_new_sharp_click() {
return (HapticController){.waveform_id = 1LL, .intensity_percent = 100LL, .duration_ms = 12LL};
}

int HapticController_trigger_haptic(HapticController* self) {
if (((*self).duration_ms > 0LL)) {
return 1;
} else {
return 0;
}
}

