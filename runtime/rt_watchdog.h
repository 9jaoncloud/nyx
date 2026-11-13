#ifndef RT_WATCHDOG_H
#define RT_WATCHDOG_H

#include <stdint.h>
#include <stdbool.h>
#include "rt_arena.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── Hardware Cycle Counter Intrinsics ── */
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
    #include <intrin.h>
    static inline uint64_t nyx_read_hw_cycles(void) {
        return __rdtsc();
    }
#elif defined(__x86_64__) || defined(__i386__)
    #include <x86intrin.h>
    static inline uint64_t nyx_read_hw_cycles(void) {
        return __rdtsc();
    }
#elif defined(__aarch64__)
    static inline uint64_t nyx_read_hw_cycles(void) {
        uint64_t val;
        __asm__ __volatile__("mrs %0, cntvct_el0" : "=r"(val));
        return val;
    }
#elif defined(__riscv)
    static inline uint64_t nyx_read_hw_cycles(void) {
        uint64_t val;
        __asm__ __volatile__("rdcycle %0" : "=r"(val));
        return val;
    }
#else
    static inline uint64_t nyx_read_hw_cycles(void) {
        return (uint64_t)clock();
    }
#endif

/* ── Fail-Safe Action Enum ── */
typedef enum NyxFailSafeAction {
    NYX_FAILSAFE_REWIND = 0,             /* Zero-cost memory arena rewind to last verified checkpoint */
    NYX_FAILSAFE_HOLD_ACTUATORS = 1,      /* Freeze control surface PWM / CAN bus commands */
    NYX_FAILSAFE_BALLISTIC_DESCENT = 2   /* Trigger emergency ballistic parachute / safe envelope */
} NyxFailSafeAction;

/* ── Hardware Watchdog State Frame ── */
typedef struct NyxWatchdog {
    uint64_t max_cycles;
    uint64_t start_cycles;
    Arena* arena;
    ArenaCheckpoint checkpoint;
    NyxFailSafeAction on_violation;
    uint64_t violations_tripped;
    bool active;
} NyxWatchdog;

/* Watchdog Enter / Exit */
void nyx_watchdog_enter(NyxWatchdog* wd, Arena* arena, uint64_t max_cycles, NyxFailSafeAction action);
bool nyx_watchdog_exit(NyxWatchdog* wd);

/* Explicit Check for Mid-Loop Budget Verification */
bool nyx_watchdog_check(NyxWatchdog* wd);

/* Trigger Fail-Safe Manually or on Fault */
void nyx_watchdog_trigger_failsafe(NyxWatchdog* wd);

#ifdef __cplusplus
}
#endif

#endif /* RT_WATCHDOG_H */
