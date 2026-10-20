#include "runtime/rt_concurrency.h"
#include "runtime/rt_http.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_json.h"
#include "runtime/rt_map.h"
#include "runtime/rt_io.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "runtime/rt_async.h"
#include <stdlib.h>
#include <string.h>
#include "runtime/rt_string.h"
#include "runtime/rt_db.h"
#include "runtime/rt_option.h"
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

#define TAG_CarScreenLayout_StandardWidescreen 0
#define TAG_CarScreenLayout_UltraWideInstrument 1
#define TAG_CarScreenLayout_HeadsUpDisplay 2
typedef struct {
    int tag;
    union {
    } data;
} CarScreenLayout;

#define TAG_AutoPlatform_AppleCarPlay 0
#define TAG_AutoPlatform_AndroidAuto 1
#define TAG_AutoPlatform_NyxSovereignCockpit 2
typedef struct {
    int tag;
    union {
    } data;
} AutoPlatform;

#define TAG_NavigationManeuver_TurnRight 0
#define TAG_NavigationManeuver_TurnLeft 1
#define TAG_NavigationManeuver_KeepStraight 2
#define TAG_NavigationManeuver_RoundaboutExit 3
#define TAG_NavigationManeuver_ArrivedAtDestination 4
typedef struct {
    int tag;
    union {
    } data;
} NavigationManeuver;

typedef struct {
    NavigationManeuver maneuver;
    rt_string_t primary_instruction;
    rt_string_t secondary_instruction;
    int64_t distance_remaining_meters;
    int64_t eta_epoch_seconds;
} ManeuverInstruction;

typedef struct {
    rt_string_t name;
    rt_string_t address;
    double distance_km;
    int64_t available_kwh;
    int64_t available_plugs;
    int64_t price_per_kwh_cents;
} ChargingStationPoi;

typedef struct {
    double speed_kmh;
    int64_t engine_rpm;
    double battery_state_of_charge_pct;
    double estimated_range_km;
    NyxVec tire_pressure_bar;
    double ambient_temp_celsius;
    int is_seatbelt_fastened;
} VehicleTelemetry;

typedef struct {
    AutoPlatform platform;
    int is_projecting;
    CarScreenLayout screen_layout;
    NyxOption active_maneuver;
} AutoCockpitSession;

AutoCockpitSession AutoCockpitSession_new(AutoPlatform);
int AutoCockpitSession_start_projection(void*);
void AutoCockpitSession_update_turn_by_turn(void*, ManeuverInstruction);
int64_t AutoCockpitSession_display_ev_charging_grid(void*, NyxVec);
VehicleTelemetry AutoCockpitSession_parse_obd_telemetry(NyxVec);
// module auto
// import std::string
// import std::vec
// import std::option
// import std::result
AutoCockpitSession AutoCockpitSession_new(AutoPlatform platform) {
return (AutoCockpitSession){.platform = platform, .is_projecting = 0, .screen_layout = (CarScreenLayout){ .tag = TAG_CarScreenLayout_StandardWidescreen }, .active_maneuver = rt_option_none()};
}

int AutoCockpitSession_start_projection(void* self) {
(self->is_projecting = 1);
return 1;
}

void AutoCockpitSession_update_turn_by_turn(void* self, ManeuverInstruction instruction) {
(self->active_maneuver = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ ManeuverInstruction _stmp_ = instruction; &_stmp_; }); _tmp; }));
}

int64_t AutoCockpitSession_display_ev_charging_grid(void* self, NyxVec stations) {
return stations.len;
}

VehicleTelemetry AutoCockpitSession_parse_obd_telemetry(NyxVec raw_can_bytes) {
return (VehicleTelemetry){.speed_kmh = 88.5, .engine_rpm = 2100LL, .battery_state_of_charge_pct = 78.4, .estimated_range_km = 342, .tire_pressure_bar = rt_vec_new(sizeof(int64_t)), .ambient_temp_celsius = 22.5, .is_seatbelt_fastened = 1};
}

