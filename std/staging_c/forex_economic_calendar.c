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

#define TAG_EconomicImpact_High 0
#define TAG_EconomicImpact_Medium 1
#define TAG_EconomicImpact_Low 2
#define TAG_EconomicImpact_Holiday 3
typedef struct {
    int tag;
    union {
    } data;
} EconomicImpact;

typedef struct {
    rt_string_t title;
    rt_string_t country;
    EconomicImpact impact;
    rt_string_t forecast;
    rt_string_t previous;
    rt_string_t actual;
    rt_string_t date_iso;
    int is_central_bank_decision;
} EconomicEvent;

typedef struct {
    rt_string_t symbol;
    double bid;
    double ask;
    double rate;
    double spread_pips;
    double change_24h_pct;
} FxPairQuote;

typedef struct {
    int64_t active_events_count;
    int64_t high_impact_count;
    double usd_ngn_rate;
    double eur_usd_rate;
    double gbp_usd_rate;
    double brent_crude_usd;
    double cbn_mpr_rate_pct;
    double us_fed_funds_rate_pct;
} MacroEconomicCalendar;

MacroEconomicCalendar MacroEconomicCalendar_new();
double MacroEconomicCalendar_calculate_fx_carry_differential(MacroEconomicCalendar*);
double MacroEconomicCalendar_compute_macro_volatility_risk(MacroEconomicCalendar*, int);
int MacroEconomicCalendar_verify_smt_risk_invariant(MacroEconomicCalendar*, double);
// module forex_economic_calendar
// import prelude::*
// import std::time::*
MacroEconomicCalendar MacroEconomicCalendar_new() {
return (MacroEconomicCalendar){.active_events_count = (int64_t)(5LL), .high_impact_count = (int64_t)(4LL), .usd_ngn_rate = 1331.28, .eur_usd_rate = 1.125, .gbp_usd_rate = 1.3232, .brent_crude_usd = 78.45, .cbn_mpr_rate_pct = 27.25, .us_fed_funds_rate_pct = 4.75};
}

double MacroEconomicCalendar_calculate_fx_carry_differential(MacroEconomicCalendar* self) {
return ((*self).cbn_mpr_rate_pct - (*self).us_fed_funds_rate_pct);
}

double MacroEconomicCalendar_compute_macro_volatility_risk(MacroEconomicCalendar* self, int has_high_impact_news_today) {
double base_risk = 0.015;
if (has_high_impact_news_today) {
return (base_risk * 2.4);
} else {
return base_risk;
}
}

int MacroEconomicCalendar_verify_smt_risk_invariant(MacroEconomicCalendar* self, double max_allowed_exposure_pct) {
double carry_spread = calculate_fx_carry_differential(self);
if (((carry_spread > 0) && (max_allowed_exposure_pct <= 2))) {
return 1;
} else {
return 0;
}
}

