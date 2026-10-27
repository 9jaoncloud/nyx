#include "runtime/rt_json.h"
#include <math.h>
#include "runtime/rt_vec.h"
#include <string.h>
#include "runtime/rt_concurrency.h"
#include "runtime/rt_string.h"
#include <stdint.h>
#include "runtime/rt_io.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include "runtime/rt_option.h"
#include "runtime/rt_map.h"
#include <stdlib.h>
#include "runtime/rt_async.h"
#include <stdio.h>
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

