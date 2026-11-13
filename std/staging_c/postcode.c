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
    rt_string_t code;
    int64_t zone_id;
    rt_string_t zone_name;
    rt_string_t state;
    rt_string_t lga;
    rt_string_t district;
    int is_valid;
    double lat;
    double lon;
} NigeriaPostcode;

typedef struct {
    rt_string_t country_code;
    rt_string_t postal_code;
    int is_valid;
    rt_string_t format_example;
} GlobalPostcodeValidation;

int validate_nigerian_postcode(rt_string_t);
NigeriaPostcode decode_nigerian_postcode(rt_string_t);
GlobalPostcodeValidation validate_global_postcode(rt_string_t, rt_string_t);
double calculate_postcode_distance(double, double, double, double);
// module postcode
// import std::io
// import std::vec
// import std::gis::geo
// extern rt_postcode_validate_ng
// extern rt_postcode_get_zone_ng
// extern rt_postcode_get_state_ng
// extern rt_postcode_validate_upu
// extern rt_postcode_distance_km
int validate_nigerian_postcode(rt_string_t code) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_postcode_validate_ng(code);
}

NigeriaPostcode decode_nigerian_postcode(rt_string_t code) {
Arena* __arena = arena_create();
int valid = rt_postcode_validate_ng(code);
int64_t zone = rt_postcode_get_zone_ng(code);
rt_string_t state = rt_postcode_get_state_ng(code);
rt_string_t z_name = rt_string_from("Unknown Zone");
({ if ((zone == 1LL)) {
z_name = rt_string_from("Zone 1 (Lagos / South-West)");
}
});
({ if ((zone == 2LL)) {
z_name = rt_string_from("Zone 2 (Ogun, Oyo, Osun, Ondo, Ekiti)");
}
});
({ if ((zone == 3LL)) {
z_name = rt_string_from("Zone 3 (Edo, Delta)");
}
});
({ if ((zone == 4LL)) {
z_name = rt_string_from("Zone 4 (Rivers, Bayelsa, Akwa Ibom, Cross River)");
}
});
({ if ((zone == 5LL)) {
z_name = rt_string_from("Zone 5 (Enugu, Anambra, Imo, Abia, Ebonyi)");
}
});
({ if ((zone == 6LL)) {
z_name = rt_string_from("Zone 6 (Benue, Plateau, Nasarawa, Kogi)");
}
});
({ if ((zone == 7LL)) {
z_name = rt_string_from("Zone 7 (Kano, Kaduna, Katsina, Jigawa)");
}
});
({ if ((zone == 8LL)) {
z_name = rt_string_from("Zone 8 (Sokoto, Kebbi, Zamfara, Niger, Kwara)");
}
});
({ if ((zone == 9LL)) {
z_name = rt_string_from("Zone 9 (Abuja FCT, Borno, Yobe, Adamawa, Bauchi, Gombe, Taraba)");
}
});
arena_destroy(__arena);
return (NigeriaPostcode){.code = code, .zone_id = zone, .zone_name = z_name, .state = state, .lga = rt_string_from("Primary Hub"), .district = rt_string_from("Central Dispatch"), .is_valid = valid, .lat = 9.0765, .lon = 7.3986};
}

GlobalPostcodeValidation validate_global_postcode(rt_string_t country_iso, rt_string_t code) {
Arena* __arena = arena_create();
int valid = rt_postcode_validate_upu(country_iso, code);
arena_destroy(__arena);
return (GlobalPostcodeValidation){.country_code = country_iso, .postal_code = code, .is_valid = valid, .format_example = rt_string_from("Standard UPU S42 Format")};
}

double calculate_postcode_distance(double lat1, double lon1, double lat2, double lon2) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_postcode_distance_km(lat1, lon1, lat2, lon2);
}

