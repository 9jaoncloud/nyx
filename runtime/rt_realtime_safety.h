/*
 * rt_realtime_safety.h — Nyx Hard-Real-Time Avionics & Automotive Safety Suite
 *
 * Implements DO-178C Level A & ISO-26262 ASIL-D Verification Architecture:
 *  1. Worst-Case Execution Time (WCET) Static Bounded Latency Validator (< 50 us)
 *  2. Zero-Heap Allocation Enforcer (#[avionics_critical]: 0 heap calls inside loops)
 *  3. Triple Modular Redundancy (TMR) & 2-out-of-3 Hardware Radiation Fault Voter
 *  4. Actuator Watchdog Timer (WDT) Failsafe Heartbeat Monitor
 */

#ifndef RT_REALTIME_SAFETY_H
#define RT_REALTIME_SAFETY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Triple Modular Redundancy (TMR) Voter ────────────────────────────── */
typedef struct {
    float channel_a;
    float channel_b;
    float channel_c;
} NyxTmrInput;

typedef struct {
    float voted_value;
    bool has_fault;
    int faulty_channel; /* 0 for none, 1 for A, 2 for B, 3 for C */
} NyxTmrResult;

NyxTmrResult nyx_tmr_vote_float(NyxTmrInput input, float tolerance);

/* ── 2. Flight Control Actuator Loop & WCET Timing Validator ─────────────── */
typedef struct {
    float target_pitch_deg;
    float current_pitch_deg;
    float elevator_command_deg;
    uint64_t max_wcet_cycles;
    uint64_t last_cycle_count;
    bool deadline_met;
    bool zero_alloc_passed;
} NyxAvionicsFlightLoop;

void nyx_avionics_loop_init(NyxAvionicsFlightLoop* loop, float target_pitch);
void nyx_avionics_step_critical(NyxAvionicsFlightLoop* loop, float measured_pitch, float kp, float kd);

/* ── 3. Watchdog Timer (WDT) Heartbeat Failsafe ──────────────────────────── */
typedef struct {
    uint32_t timeout_ticks;
    uint32_t ticks_since_last_feed;
    bool is_tripped;
    bool failsafe_engaged;
} NyxWatchdogTimer;

void nyx_wdt_init(NyxWatchdogTimer* wdt, uint32_t timeout_ticks);
void nyx_wdt_feed(NyxWatchdogTimer* wdt);
void nyx_wdt_tick(NyxWatchdogTimer* wdt);

#ifdef __cplusplus
}
#endif

#endif /* RT_REALTIME_SAFETY_H */
