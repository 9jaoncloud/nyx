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

#define TAG_HapticPattern_LightImpact 0
#define TAG_HapticPattern_MediumImpact 1
#define TAG_HapticPattern_HeavyImpact 2
#define TAG_HapticPattern_SelectionChange 3
#define TAG_HapticPattern_SuccessNotification 4
#define TAG_HapticPattern_WarningNotification 5
#define TAG_HapticPattern_ErrorNotification 6
typedef struct {
    int tag;
    union {
    } data;
} HapticPattern;

#define TAG_GestureType_Tap 0
#define TAG_GestureType_DoubleTap 1
#define TAG_GestureType_LongPress 2
#define TAG_GestureType_Pan 3
#define TAG_GestureType_PinchZoom 4
#define TAG_GestureType_Swipe 5
typedef struct {
    int tag;
    union {
    } data;
} GestureType;

#define TAG_TouchPhase_Began 0
#define TAG_TouchPhase_Moved 1
#define TAG_TouchPhase_Stationary 2
#define TAG_TouchPhase_Ended 3
#define TAG_TouchPhase_Cancelled 4
typedef struct {
    int tag;
    union {
    } data;
} TouchPhase;

#define TAG_BatteryState_Unplugged 0
#define TAG_BatteryState_Charging 1
#define TAG_BatteryState_Full 2
#define TAG_BatteryState_Unknown 3
typedef struct {
    int tag;
    union {
    } data;
} BatteryState;

#define TAG_Orientation_Portrait 0
#define TAG_Orientation_PortraitUpsideDown 1
#define TAG_Orientation_LandscapeLeft 2
#define TAG_Orientation_LandscapeRight 3
typedef struct {
    int tag;
    union {
    } data;
} Orientation;

typedef struct {
    int64_t id;
    double x;
    double y;
    double pressure;
    TouchPhase phase;
} TouchPointer;

typedef struct {
    GestureType gesture;
    double focal_x;
    double focal_y;
    double scale;
    double velocity_x;
    double velocity_y;
} GestureEvent;

typedef struct {
    double top;
    double bottom;
    double left;
    double right;
} SafeAreaInsets;

typedef struct {
    int64_t physical_width_px;
    int64_t physical_height_px;
    double logical_width_dp;
    double logical_height_dp;
    double scale_density;
    double refresh_rate_hz;
    Orientation orientation;
    SafeAreaInsets safe_area;
} DisplayMetrics;

typedef struct {
    double x;
    double y;
    double z;
} Vector3D;

typedef struct {
    Vector3D acceleration_m_s2;
    Vector3D gyroscope_rad_s;
    Vector3D magnetometer_ut;
    int64_t timestamp_us;
} SensorData;

typedef struct {
    double level_percent;
    BatteryState state;
    int is_low_power_mode;
} BatteryInfo;

NyxResult get_display_metrics();
DisplayMetrics reference_display_metrics();
NyxResult read_sensors();
SensorData reference_sensor_data();
NyxResult get_battery_info();
BatteryInfo reference_battery_info();
void trigger_haptic(HapticPattern);
// module mobile
// import std::string
// import std::vec
// import std::option
// import std::result
// import std::mobile::runtime
// import std::mobile::nfc
// import std::mobile::ble
// import std::mobile::camera
// import std::mobile::sync
// import std::mobile::push
// import std::mobile::billing
// import std::mobile::audio
// import std::mobile::biometrics
// import std::mobile::auto
// import std::mobile::a11y
// import std::mobile::npu_slm
// import std::mobile::pqc_enclave
// import std::mobile::trifold
// import std::mobile::spatial_teleport
// import std::mobile::hologram
// import std::mobile::ios_metal
// import std::mobile::bifrost
// import std::mobile::android_compat
// import std::mobile::satellite_ntn
// import std::mobile::neuromorphic
// import std::mobile::zk_identity
// import std::ipa
NyxResult get_display_metrics() {
Arena* __arena = arena_create();
NyxResult r = require((MobileCapability){ .tag = TAG_MobileCapability_Display });
arena_destroy(__arena);
return ({
NyxResult __match_result_0;
NyxResult __match_val_0 = r;
if (__match_val_0.tag == RESULT_OK) {
__match_result_0 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ DisplayMetrics _stmp_ = reference_display_metrics(); &_stmp_; }); _tmp; });
}
 else if (__match_val_0.tag == RESULT_ERR) {
    rt_string_t e = rt_string_from((char*)__match_val_0.data.ptr_val);
__match_result_0 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({{ rt_string_t _stmp_ = e; _stmp_.data; }); _tmp; });
}
__match_result_0;
});
}

DisplayMetrics reference_display_metrics() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (DisplayMetrics){.physical_width_px = 1170LL, .physical_height_px = 2532LL, .logical_width_dp = 390, .logical_height_dp = 844, .scale_density = 3, .refresh_rate_hz = 120, .orientation = (Orientation){ .tag = TAG_Orientation_Portrait }, .safe_area = (SafeAreaInsets){.top = 47, .bottom = 34, .left = 0, .right = 0}};
}

NyxResult read_sensors() {
Arena* __arena = arena_create();
NyxResult r = require((MobileCapability){ .tag = TAG_MobileCapability_Sensors });
arena_destroy(__arena);
return ({
NyxResult __match_result_1;
NyxResult __match_val_1 = r;
if (__match_val_1.tag == RESULT_OK) {
__match_result_1 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ SensorData _stmp_ = reference_sensor_data(); &_stmp_; }); _tmp; });
}
 else if (__match_val_1.tag == RESULT_ERR) {
    rt_string_t e = rt_string_from((char*)__match_val_1.data.ptr_val);
__match_result_1 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({{ rt_string_t _stmp_ = e; _stmp_.data; }); _tmp; });
}
__match_result_1;
});
}

SensorData reference_sensor_data() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (SensorData){.acceleration_m_s2 = (Vector3D){.x = 0.02, .y = 9.81, .z = 0.05}, .gyroscope_rad_s = (Vector3D){.x = 0.001, .y = 0.002, .z = 0}, .magnetometer_ut = (Vector3D){.x = 22.4, .y = (-5.1), .z = 42.8}, .timestamp_us = 0LL};
}

NyxResult get_battery_info() {
Arena* __arena = arena_create();
NyxResult r = require((MobileCapability){ .tag = TAG_MobileCapability_Battery });
arena_destroy(__arena);
return ({
NyxResult __match_result_2;
NyxResult __match_val_2 = r;
if (__match_val_2.tag == RESULT_OK) {
__match_result_2 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ BatteryInfo _stmp_ = reference_battery_info(); &_stmp_; }); _tmp; });
}
 else if (__match_val_2.tag == RESULT_ERR) {
    rt_string_t e = rt_string_from((char*)__match_val_2.data.ptr_val);
__match_result_2 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({{ rt_string_t _stmp_ = e; _stmp_.data; }); _tmp; });
}
__match_result_2;
});
}

BatteryInfo reference_battery_info() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (BatteryInfo){.level_percent = 0, .state = (BatteryState){ .tag = TAG_BatteryState_Unknown }, .is_low_power_mode = 0};
}

// extern nyx_mobile_haptic_trigger
void trigger_haptic(HapticPattern pattern) {
Arena* __arena = arena_create();
NyxResult cap = require((MobileCapability){ .tag = TAG_MobileCapability_Haptics });
({ if (({ NyxResult _r = cap; _r.tag == RESULT_ERR; })) {
return;
}
});
int64_t code = ({
int64_t __match_result_4;
HapticPattern __match_val_4 = pattern;
if (__match_val_4.tag == TAG_HapticPattern_LightImpact) {
__match_result_4 = 1LL;
}
 else if (__match_val_4.tag == TAG_HapticPattern_MediumImpact) {
__match_result_4 = 2LL;
}
 else if (__match_val_4.tag == TAG_HapticPattern_HeavyImpact) {
__match_result_4 = 3LL;
}
 else if (__match_val_4.tag == TAG_HapticPattern_SelectionChange) {
__match_result_4 = 4LL;
}
 else if (__match_val_4.tag == TAG_HapticPattern_SuccessNotification) {
__match_result_4 = 5LL;
}
 else if (__match_val_4.tag == TAG_HapticPattern_WarningNotification) {
__match_result_4 = 6LL;
}
 else if (__match_val_4.tag == TAG_HapticPattern_ErrorNotification) {
__match_result_4 = 7LL;
}
__match_result_4;
});
({
nyx_mobile_haptic_trigger(code);
});
arena_destroy(__arena);
}

