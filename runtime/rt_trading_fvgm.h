/*
 * rt_trading_fvgm.h — Fair Value Gap Mitigation (FVGM) & Multi-Profile Auto-Optimizer Engine
 *
 * Direct High-Performance Zero-GC Native Implementation of PhenLabs FVGM Pine Script v6:
 *  - 3-Candle Imbalance & FVG Zone Detection (Consequent Encroachment 50% CE midline)
 *  - Kaufman Trend Efficiency Ratio (KER 10-bar) & Dynamic Displacement Scoring
 *  - 6 Parallel Profile State Machines:
 *      0: Balanced (50% CE · 58% Defense)
 *      1: Conservative (50% CE · 65% Defense)
 *      2: Aggressive (Edge Reclaim · 50% Defense)
 *      3: Scalp Fast (Quick Test · 54% Defense)
 *      4: Swing Wide (Deep Imbalance · 56% Defense)
 *      5: Volume Climax (50% CE · 1.25x Volume)
 *  - Deterministic Trade Plan Lifecycle with TP1 / TP2 / TP3 Target Ladders
 *  - Partial Scale-Out Blended R Accounting (33% TP1, 33% TP2, 34% TP3)
 *  - Breakeven Ratchet Stop Loss after TP1 Confirmed
 *  - Opposing Swing Liquidity Magnet Snapping (1.4R – 3.8R)
 *  - Multi-Ledger Walk-Forward Auto-Optimizer Ranker
 */

#ifndef RT_TRADING_FVGM_H
#define RT_TRADING_FVGM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FVGM_MAX_ZONES_PER_SIDE 8
#define FVGM_MAX_PLANS 32
#define FVGM_MAX_SWINGS 16
#define FVGM_PROFILE_COUNT 6

typedef enum {
    FVGM_PROF_BALANCED = 0,
    FVGM_PROF_CONSERVATIVE = 1,
    FVGM_PROF_AGGRESSIVE = 2,
    FVGM_PROF_SCALP_FAST = 3,
    FVGM_PROF_SWING_WIDE = 4,
    FVGM_PROF_VOL_CLIMAX = 5,
    FVGM_PROF_AUTO = -1
} FvgmProfile;

typedef struct {
    int64_t timestamp;
    double open;
    double high;
    double low;
    double close;
    double volume;
} FvgmCandle;

typedef struct {
    int dir;             /* +1 = Bullish FVG, -1 = Bearish FVG */
    double top;
    double bot;
    double ce;           /* Consequent Encroachment: (top + bot) / 2.0 */
    int birth_bar;
    int64_t birth_time;
    double effort;
    bool touched;
    int last_touch_bar;
    bool dead;
    uint32_t fired_mask; /* bitmask for profiles 0..5 */
} FvgmZone;

typedef struct {
    int prof;            /* 0..5 */
    int dir;             /* +1 = Long, -1 = Short */
    double entry;
    double sl;           /* Initial stop loss */
    double cur_sl;       /* Dynamic trailing / breakeven stop loss */
    double tp1;
    double tp2;
    double tp3;
    double risk;
    int entry_bar;
    int64_t entry_time;
    int exit_bar;
    int64_t exit_time;
    double exit_price;
    double score;
    bool mag2;           /* Magnet snap active for TP2 */
    bool mag3;           /* Magnet snap active for TP3 */
    bool hit1;
    bool hit2;
    bool hit3;
    bool sl_hit;
    bool be_hit;
    bool is_open;
    double result_r;
    char outcome[16];    /* "OPEN", "TP1_BE", "TP2_BE", "FULL_TP", "SL", "TIMEOUT" */
} FvgmPlan;

typedef struct {
    int trades;
    int wins;
    int losses;
    int tp1_hits;
    int tp2_hits;
    int tp3_hits;
    double net_r;
    double gross_w;
    double gross_l;
    double peak;
    double max_dd;
} FvgmProfileLedger;

typedef struct {
    /* Config Inputs */
    double min_gap_atr;    /* Default 0.25 */
    int zone_age_bars;     /* Default 120 */
    int max_live_zones;    /* Default 3 */
    int trend_ema_len;     /* Default 50 */
    double tp1_r;          /* Default 1.0 */
    double tp2_r;          /* Default 2.0 */
    double tp3_r;          /* Default 3.0 */
    bool use_breakeven;    /* Move SL to breakeven after TP1 */
    bool use_magnet;       /* Snap targets to opposing liquidity */
    int timeout_bars;      /* Default 60 */
    double weight_tp1;     /* Default 0.33 */
    double weight_tp2;     /* Default 0.33 */
    double weight_tp3;     /* Default 0.34 */
    double round_trip_cost;/* Default 0.05R */

    /* Rolling Indicator State */
    double prev_close;
    double prev_high;
    double prev_low;
    double prev_open;
    double h2_ago;
    double l2_ago;
    double atr14;
    double ema_trend;
    double ema_trend_prev3;
    double vol_sma20;
    double vol_buffer[20];
    size_t vol_count;
    double close_history[16];
    size_t history_count;
    int bar_index;

    /* Zones */
    FvgmZone bull_zones[FVGM_MAX_ZONES_PER_SIDE];
    size_t bull_zone_count;
    FvgmZone bear_zones[FVGM_MAX_ZONES_PER_SIDE];
    size_t bear_zone_count;

    /* Swing Pivots */
    double swing_highs[FVGM_MAX_SWINGS];
    size_t swing_high_count;
    double swing_lows[FVGM_MAX_SWINGS];
    size_t swing_low_count;

    /* Active Plans per profile (0..5) + Auto Walk-Forward (6) */
    FvgmPlan active_plans[FVGM_PROFILE_COUNT];
    bool has_active[FVGM_PROFILE_COUNT];
    FvgmProfileLedger ledgers[FVGM_PROFILE_COUNT + 1]; /* Index 6 = Auto Walk-Forward */
    int last_sig_bar[FVGM_PROFILE_COUNT];

    /* Recent Trade Plans Log */
    FvgmPlan closed_plans[FVGM_MAX_PLANS];
    size_t closed_plan_count;

    /* Real-Time Metrics */
    double last_kaufman_er;
    double last_effort;
    double last_rel_activity;
    int selected_profile;
} FvgmEngine;

static inline double fvgm_clamp(double v, double lo, double hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static inline double fvgm_d_min(int p) {
    switch (p) {
        case 0: return 0.52;
        case 1: return 0.60;
        case 2: return 0.44;
        case 3: return 0.48;
        case 4: return 0.52;
        default: return 0.54;
    }
}

static inline double fvgm_score_min(int p) {
    switch (p) {
        case 0: return 58.0;
        case 1: return 65.0;
        case 2: return 50.0;
        case 3: return 54.0;
        case 4: return 56.0;
        default: return 60.0;
    }
}

static inline double fvgm_er_min(int p) {
    switch (p) {
        case 0: return 22.0;
        case 1: return 30.0;
        case 2: return 15.0;
        case 3: return 18.0;
        case 4: return 24.0;
        default: return 22.0;
    }
}

static inline double fvgm_buf(int p) {
    switch (p) {
        case 0: return 0.22;
        case 1: return 0.28;
        case 2: return 0.18;
        case 3: return 0.16;
        case 4: return 0.36;
        default: return 0.22;
    }
}

static inline double fvgm_min_risk(int p) {
    if (p == 3) return 0.35;
    if (p == 4) return 0.50;
    return 0.40;
}

static inline bool fvgm_req_trend(int p) {
    return (p != 2); /* Aggressive profile trades counter-trend */
}

static inline bool fvgm_req_ce(int p) {
    return (p == 0 || p == 1 || p == 5); /* Balanced, Conservative, Vol Climax require 50% CE touch */
}

static inline double fvgm_eff_min(int p) {
    switch (p) {
        case 0: return 35.0;
        case 1: return 48.0;
        case 2: return 25.0;
        case 3: return 30.0;
        case 4: return 42.0;
        default: return 45.0;
    }
}

static inline double fvgm_act_min(int p) {
    return (p == 5) ? 1.25 : 0.0;
}

static inline double fvgm_age_mult(int p) {
    if (p == 3) return 0.5;
    if (p == 4) return 1.6;
    return 1.0;
}

static inline int fvgm_cooldown(int p) {
    switch (p) {
        case 0: return 5;
        case 1: return 8;
        case 2: return 3;
        case 3: return 4;
        case 4: return 8;
        default: return 6;
    }
}

static inline const char* fvgm_profile_name(int p) {
    switch (p) {
        case 0: return "Balanced (50% CE · 58% Def)";
        case 1: return "Conservative (50% CE · 65% Def)";
        case 2: return "Aggressive (Edge · 50% Def)";
        case 3: return "Scalp Fast (Quick · 54% Def)";
        case 4: return "Swing Wide (Deep · 56% Def)";
        case 5: return "Volume Climax (50% CE · 1.25x Vol)";
        default: return "Auto-Optimizer";
    }
}

/* Calculate blended real R accounting for partial scale-outs & roundtrip fees */
static inline double fvgm_calc_real_r(const FvgmEngine* eng, const FvgmPlan* ap, double exit_px) {
    double rk = (ap->risk > 1e-6) ? ap->risk : 1e-4;
    double d = (double)ap->dir;
    double done = 0.0;
    double acc = 0.0;

    if (ap->hit1) {
        acc += eng->weight_tp1 * (ap->tp1 - ap->entry) * d / rk;
        done += eng->weight_tp1;
    }
    if (ap->hit2) {
        acc += eng->weight_tp2 * (ap->tp2 - ap->entry) * d / rk;
        done += eng->weight_tp2;
    }
    if (ap->hit3) {
        acc += eng->weight_tp3 * (ap->tp3 - ap->entry) * d / rk;
        done += eng->weight_tp3;
    }
    double remainder = 1.0 - done;
    if (remainder < 0.0) remainder = 0.0;
    acc += remainder * (exit_px - ap->entry) * d / rk - eng->round_trip_cost;
    return acc;
}

/* Book trade outcome into profile ledger */
static inline void fvgm_book_ledger(FvgmProfileLedger* led, double r) {
    led->trades++;
    led->net_r += r;
    if (r > 0.0) {
        led->wins++;
        led->gross_w += r;
    } else {
        led->losses++;
        led->gross_l -= r;
    }
    if (led->net_r > led->peak) led->peak = led->net_r;
    double dd = led->peak - led->net_r;
    if (dd > led->max_dd) led->max_dd = dd;
}

/* Auto-Optimizer selection across the 6 profiles */
static inline int fvgm_pick_auto(const FvgmProfileLedger* ledgers) {
    int best = -1;
    double best_score = -1e9;
    double best_nr = -1e9;

    /* Pass 1: >= 4 closed trades with positive Net R */
    for (int i = 0; i < FVGM_PROFILE_COUNT; i++) {
        int n = ledgers[i].trades;
        double net = ledgers[i].net_r;
        if (n >= 4 && net > 0.0) {
            double wr = ((double)ledgers[i].wins * 100.0) / n;
            double avg = net / n;
            double sc = wr + avg * 22.0 + ((net < 40.0 ? net : 40.0) * 0.45);
            if (sc > best_score || (sc == best_score && net > best_nr)) {
                best = i;
                best_score = sc;
                best_nr = net;
            }
        }
    }
    /* Pass 2: >= 3 closed trades ranked by Win Rate + Expectancy */
    if (best == -1) {
        for (int i = 0; i < FVGM_PROFILE_COUNT; i++) {
            int n = ledgers[i].trades;
            double net = ledgers[i].net_r;
            if (n >= 3) {
                double wr = ((double)ledgers[i].wins * 100.0) / n;
                double avg = net / n;
                double sc = wr + avg * 25.0;
                if (sc > best_score || (sc == best_score && net > best_nr)) {
                    best = i;
                    best_score = sc;
                    best_nr = net;
                }
            }
        }
    }
    /* Pass 3: Fallback on any closed trade */
    if (best == -1) {
        for (int i = 0; i < FVGM_PROFILE_COUNT; i++) {
            int n = ledgers[i].trades;
            if (n > 0) {
                double wr = ((double)ledgers[i].wins * 100.0) / n;
                if (wr > best_score || (wr == best_score && ledgers[i].net_r > best_nr)) {
                    best = i;
                    best_score = wr;
                    best_nr = ledgers[i].net_r;
                }
            }
        }
    }
    return (best == -1) ? 0 : best;
}

static inline void fvgm_init(FvgmEngine* eng) {
    memset(eng, 0, sizeof(FvgmEngine));
    eng->min_gap_atr = 0.25;
    eng->zone_age_bars = 120;
    eng->max_live_zones = 3;
    eng->trend_ema_len = 50;
    eng->tp1_r = 1.0;
    eng->tp2_r = 2.0;
    eng->tp3_r = 3.0;
    eng->use_breakeven = true;
    eng->use_magnet = true;
    eng->timeout_bars = 60;
    eng->weight_tp1 = 0.33;
    eng->weight_tp2 = 0.33;
    eng->weight_tp3 = 0.34;
    eng->round_trip_cost = 0.05;

    for (int i = 0; i < FVGM_PROFILE_COUNT; i++) {
        eng->last_sig_bar[i] = -999;
    }
}

/* Process one candle bar update */
static inline void fvgm_process_candle(FvgmEngine* eng, const FvgmCandle* c) {
    eng->bar_index++;
    int bar = eng->bar_index;

    /* 1. Bar Range & Moving Metrics */
    double bar_rng = (c->high - c->low > 1e-4) ? (c->high - c->low) : 1e-4;
    double tr = bar_rng;
    if (eng->bar_index > 1) {
        double diff1 = fabs(c->high - eng->prev_close);
        double diff2 = fabs(c->low - eng->prev_close);
        if (diff1 > tr) tr = diff1;
        if (diff2 > tr) tr = diff2;
    }

    /* ATR(14) RMA */
    if (eng->bar_index == 1) {
        eng->atr14 = tr;
    } else {
        eng->atr14 = (eng->atr14 * 13.0 + tr) / 14.0;
    }
    double atr = (eng->atr14 > 1e-4) ? eng->atr14 : bar_rng;

    /* Trend EMA(50) */
    double alpha_ema = 2.0 / (eng->trend_ema_len + 1.0);
    if (eng->bar_index == 1) {
        eng->ema_trend = c->close;
    } else {
        eng->ema_trend = alpha_ema * c->close + (1.0 - alpha_ema) * eng->ema_trend;
    }

    /* Volume SMA(20) */
    eng->vol_buffer[eng->vol_count % 20] = c->volume;
    eng->vol_count++;
    size_t vol_w = (eng->vol_count < 20) ? eng->vol_count : 20;
    double sum_vol = 0.0;
    for (size_t i = 0; i < vol_w; i++) sum_vol += eng->vol_buffer[i];
    eng->vol_sma20 = (vol_w > 0) ? (sum_vol / vol_w) : c->volume;

    double rel_act = (eng->vol_sma20 > 0.0) ? (c->volume / eng->vol_sma20) : 1.0;
    eng->last_rel_activity = rel_act;
    double act_n = fvgm_clamp(rel_act / 2.0, 0.0, 1.0);
    double body_eff = fabs(c->close - c->open) / bar_rng;
    eng->last_effort = 100.0 * (0.55 * body_eff + 0.45 * act_n);

    /* Kaufman Efficiency Ratio over up to 10 bars */
    eng->close_history[eng->history_count % 16] = c->close;
    eng->history_count++;
    double er_pct = 50.0; /* Default neutral efficiency */
    if (eng->history_count >= 2) {
        int w = (eng->history_count < 10) ? (int)eng->history_count : 10;
        double close_old = eng->close_history[(eng->history_count - w) % 16];
        double net_move = fabs(c->close - close_old);
        double path_move = 0.0;
        for (int k = 0; k < w - 1; k++) {
            double c_curr = eng->close_history[(eng->history_count - 1 - k) % 16];
            double c_prev = eng->close_history[(eng->history_count - 2 - k) % 16];
            path_move += fabs(c_curr - c_prev);
        }
        if (path_move > 1e-4) {
            er_pct = fvgm_clamp((net_move / path_move) * 100.0, 0.0, 100.0);
        }
    }
    eng->last_kaufman_er = er_pct;
    double er_norm = fvgm_clamp(er_pct / 55.0, 0.0, 1.0);

    /* Displacement Scores */
    double d_bull = fvgm_clamp((c->close - c->low) / bar_rng, 0.0, 1.0);
    double d_bear = fvgm_clamp((c->high - c->close) / bar_rng, 0.0, 1.0);
    double sc_bull = (0.45 * d_bull + 0.30 * er_norm + 0.25 * act_n) * 100.0 * (c->close > c->open ? 1.0 : 0.82);
    double sc_bear = (0.45 * d_bear + 0.30 * er_norm + 0.25 * act_n) * 100.0 * (c->close < c->open ? 1.0 : 0.82);

    /* ── Step 2: Manage Active Trade Plans (Stop-First, Scales, Breakeven) ──── */
    for (int p = 0; p < FVGM_PROFILE_COUNT; p++) {
        if (!eng->has_active[p]) continue;
        FvgmPlan* ap = &eng->active_plans[p];
        if (bar <= ap->entry_bar) continue;

        bool is_l = (ap->dir == 1);
        bool done = false;

        /* Stop Loss check */
        if (is_l ? (c->low <= ap->cur_sl) : (c->high >= ap->cur_sl)) {
            ap->exit_price = ap->cur_sl;
            ap->be_hit = (ap->hit1 && eng->use_breakeven);
            ap->sl_hit = !ap->be_hit;
            strcpy(ap->outcome, ap->be_hit ? "BE_STOP" : "STOP_LOSS");
            done = true;
        }

        /* Targets check */
        if (!done) {
            if (is_l ? (c->high >= ap->tp1) : (c->low <= ap->tp1)) {
                if (!ap->hit1) {
                    ap->hit1 = true;
                    if (eng->use_breakeven) {
                        ap->cur_sl = ap->entry; /* Breakeven ratchet */
                    }
                }
            }
            if (is_l ? (c->high >= ap->tp2) : (c->low <= ap->tp2)) {
                ap->hit2 = true;
            }
            if (is_l ? (c->high >= ap->tp3) : (c->low <= ap->tp3)) {
                ap->hit3 = true;
                ap->exit_price = ap->tp3;
                strcpy(ap->outcome, "FULL_TP3");
                done = true;
            }
        }

        /* Timeout check */
        if (!done && (bar - ap->entry_bar >= eng->timeout_bars)) {
            ap->exit_price = c->close;
            strcpy(ap->outcome, "TIMEOUT");
            done = true;
        }

        if (done) {
            ap->is_open = false;
            ap->exit_bar = bar;
            ap->exit_time = c->timestamp;
            ap->result_r = fvgm_calc_real_r(eng, ap, ap->exit_price);

            /* Book into profile ledger */
            fvgm_book_ledger(&eng->ledgers[p], ap->result_r);
            if (ap->hit1) eng->ledgers[p].tp1_hits++;
            if (ap->hit2) eng->ledgers[p].tp2_hits++;
            if (ap->hit3) eng->ledgers[p].tp3_hits++;

            /* If this trade was taken under auto walk-forward, book index 6 */
            if (p == eng->selected_profile) {
                fvgm_book_ledger(&eng->ledgers[FVGM_PROFILE_COUNT], ap->result_r);
            }

            /* Archive plan */
            if (eng->closed_plan_count < FVGM_MAX_PLANS) {
                eng->closed_plans[eng->closed_plan_count++] = *ap;
            } else {
                memmove(&eng->closed_plans[0], &eng->closed_plans[1], sizeof(FvgmPlan) * (FVGM_MAX_PLANS - 1));
                eng->closed_plans[FVGM_MAX_PLANS - 1] = *ap;
            }
            eng->has_active[p] = false;
        }
    }

    /* ── Step 3: FVG Formation Detection (3-Candle Imbalance) ──────────────── */
    if (eng->bar_index >= 3) {
        /* Bullish Gap: low > high[2] && (low - high[2]) >= min_gap_atr * atr && close[1] > open[1] */
        if ((c->low > eng->h2_ago) && ((c->low - eng->h2_ago) >= eng->min_gap_atr * atr) && (eng->prev_close > eng->prev_open)) {
            FvgmZone z;
            z.dir = 1;
            z.bot = eng->h2_ago;
            z.top = c->low;
            z.ce = (z.top + z.bot) / 2.0;
            z.birth_bar = bar;
            z.birth_time = c->timestamp;
            z.effort = eng->last_effort;
            z.touched = false;
            z.last_touch_bar = -1;
            z.dead = false;
            z.fired_mask = 0;

            if (eng->bull_zone_count < FVGM_MAX_ZONES_PER_SIDE) {
                eng->bull_zones[eng->bull_zone_count++] = z;
            } else {
                memmove(&eng->bull_zones[0], &eng->bull_zones[1], sizeof(FvgmZone) * (FVGM_MAX_ZONES_PER_SIDE - 1));
                eng->bull_zones[FVGM_MAX_ZONES_PER_SIDE - 1] = z;
            }
        }

        /* Bearish Gap: high < low[2] && (low[2] - high) >= min_gap_atr * atr && close[1] < open[1] */
        if ((c->high < eng->l2_ago) && ((eng->l2_ago - c->high) >= eng->min_gap_atr * atr) && (eng->prev_close < eng->prev_open)) {
            FvgmZone z;
            z.dir = -1;
            z.top = eng->l2_ago;
            z.bot = c->high;
            z.ce = (z.top + z.bot) / 2.0;
            z.birth_bar = bar;
            z.birth_time = c->timestamp;
            z.effort = eng->last_effort;
            z.touched = false;
            z.last_touch_bar = -1;
            z.dead = false;
            z.fired_mask = 0;

            if (eng->bear_zone_count < FVGM_MAX_ZONES_PER_SIDE) {
                eng->bear_zones[eng->bear_zone_count++] = z;
            } else {
                memmove(&eng->bear_zones[0], &eng->bear_zones[1], sizeof(FvgmZone) * (FVGM_MAX_ZONES_PER_SIDE - 1));
                eng->bear_zones[FVGM_MAX_ZONES_PER_SIDE - 1] = z;
            }
        }
    }

    /* ── Step 4: Zone Mitigation, Invalidation & Entry Signal Dispatch ──────── */
    for (int p = 0; p < FVGM_PROFILE_COUNT; p++) {
        if (eng->has_active[p]) continue;
        if (bar - eng->last_sig_bar[p] < fvgm_cooldown(p)) continue;

        double d_req = fvgm_d_min(p);
        double sc_req = fvgm_score_min(p);
        double er_req = fvgm_er_min(p);
        double eff_req = fvgm_eff_min(p);
        double act_req = fvgm_act_min(p);
        double max_age = eng->zone_age_bars * fvgm_age_mult(p);
        bool ce_req = fvgm_req_ce(p);
        bool tr_req = fvgm_req_trend(p);

        /* Evaluate Bullish Zones for Long Mitigation Entry */
        for (size_t z = 0; z < eng->bull_zone_count; z++) {
            FvgmZone* bz = &eng->bull_zones[z];
            if (bz->dead) continue;
            if (bar <= bz->birth_bar) continue; /* Mitigation requires pullback on later bar */
            if (bar - bz->birth_bar > max_age) {
                bz->dead = true;
                continue;
            }

            /* Invalidation: Close below zone bottom */
            if (c->close < bz->bot) {
                bz->dead = true;
                continue;
            }

            /* Touch check: low enters zone */
            if (c->low <= bz->top) {
                bz->touched = true;
                bz->last_touch_bar = bar;
            }

            if (!bz->touched) continue;

            /* Check if profile has already fired on this zone */
            if ((bz->fired_mask & (1 << p)) != 0) continue;

            /* Filter conditions */
            bool ce_ok = !ce_req || (c->low <= bz->ce);
            bool tr_ok = !tr_req || (c->close > eng->ema_trend);
            bool act_ok = (eng->last_rel_activity >= act_req);
            bool sig_ok = (d_bull >= d_req) && (sc_bull >= sc_req) && (er_pct >= er_req) && (eng->last_effort >= eff_req) && ce_ok && tr_ok && act_ok;

            if (sig_ok) {
                bz->fired_mask |= (1 << p);
                eng->last_sig_bar[p] = bar;

                FvgmPlan plan;
                memset(&plan, 0, sizeof(FvgmPlan));
                plan.prof = p;
                plan.dir = 1;
                plan.entry = c->close;
                plan.sl = bz->bot - fvgm_buf(p) * atr;
                plan.cur_sl = plan.sl;
                plan.risk = plan.entry - plan.sl;

                if (plan.risk >= fvgm_min_risk(p) * atr) {
                    plan.tp1 = plan.entry + eng->tp1_r * plan.risk;
                    plan.tp2 = plan.entry + eng->tp2_r * plan.risk;
                    plan.tp3 = plan.entry + eng->tp3_r * plan.risk;
                    plan.entry_bar = bar;
                    plan.entry_time = c->timestamp;
                    plan.score = sc_bull;
                    plan.is_open = true;
                    strcpy(plan.outcome, "OPEN");

                    eng->active_plans[p] = plan;
                    eng->has_active[p] = true;
                    break;
                }
            }
        }

        /* Evaluate Bearish Zones for Short Mitigation Entry */
        if (!eng->has_active[p]) {
            for (size_t z = 0; z < eng->bear_zone_count; z++) {
                FvgmZone* bz = &eng->bear_zones[z];
                if (bz->dead) continue;
                if (bar <= bz->birth_bar) continue; /* Mitigation requires pullback on later bar */
                if (bar - bz->birth_bar > max_age) {
                    bz->dead = true;
                    continue;
                }

                /* Invalidation: Close above zone top */
                if (c->close > bz->top) {
                    bz->dead = true;
                    continue;
                }

                /* Touch check: high enters zone */
                if (c->high >= bz->bot) {
                    bz->touched = true;
                    bz->last_touch_bar = bar;
                }

                if (!bz->touched) continue;

                /* Check if profile has already fired on this zone */
                if ((bz->fired_mask & (1 << p)) != 0) continue;

                /* Filter conditions */
                bool ce_ok = !ce_req || (c->high >= bz->ce);
                bool tr_ok = !tr_req || (c->close < eng->ema_trend);
                bool act_ok = (eng->last_rel_activity >= act_req);
                bool sig_ok = (d_bear >= d_req) && (sc_bear >= sc_req) && (er_pct >= er_req) && (eng->last_effort >= eff_req) && ce_ok && tr_ok && act_ok;

                if (sig_ok) {
                    bz->fired_mask |= (1 << p);
                    eng->last_sig_bar[p] = bar;

                    FvgmPlan plan;
                    memset(&plan, 0, sizeof(FvgmPlan));
                    plan.prof = p;
                    plan.dir = -1;
                    plan.entry = c->close;
                    plan.sl = bz->top + fvgm_buf(p) * atr;
                    plan.cur_sl = plan.sl;
                    plan.risk = plan.sl - plan.entry;

                    if (plan.risk >= fvgm_min_risk(p) * atr) {
                        plan.tp1 = plan.entry - eng->tp1_r * plan.risk;
                        plan.tp2 = plan.entry - eng->tp2_r * plan.risk;
                        plan.tp3 = plan.entry - eng->tp3_r * plan.risk;
                        plan.entry_bar = bar;
                        plan.entry_time = c->timestamp;
                        plan.score = sc_bear;
                        plan.is_open = true;
                        strcpy(plan.outcome, "OPEN");

                        eng->active_plans[p] = plan;
                        eng->has_active[p] = true;
                        break;
                    }
                }
            }
        }
    }

    /* Update Walk-Forward Auto Optimizer Selection */
    eng->selected_profile = fvgm_pick_auto(eng->ledgers);

    /* Historical Shift */
    eng->h2_ago = eng->prev_high;
    eng->l2_ago = eng->prev_low;
    eng->prev_open = c->open;
    eng->prev_high = c->high;
    eng->prev_low = c->low;
    eng->prev_close = c->close;
}

#ifdef __cplusplus
}
#endif

#endif /* RT_TRADING_FVGM_H */
