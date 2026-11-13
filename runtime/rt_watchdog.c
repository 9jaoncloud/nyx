#include "rt_watchdog.h"
#include <stdio.h>

void nyx_watchdog_enter(NyxWatchdog* wd, Arena* arena, uint64_t max_cycles, NyxFailSafeAction action) {
    if (!wd) return;
    wd->arena = arena;
    wd->max_cycles = max_cycles;
    wd->on_violation = action;
    wd->violations_tripped = 0;
    wd->active = true;

    /* Capture sub-nanosecond hardware cycle count */
    wd->start_cycles = nyx_read_hw_cycles();

    /* Checkpoint volatile region memory state */
    if (arena) {
        wd->checkpoint = arena_checkpoint(arena);
    }
}

bool nyx_watchdog_check(NyxWatchdog* wd) {
    if (!wd || !wd->active) return true;

    uint64_t now = nyx_read_hw_cycles();
    uint64_t elapsed = now - wd->start_cycles;

    if (elapsed > wd->max_cycles) {
        wd->violations_tripped++;
        nyx_watchdog_trigger_failsafe(wd);
        return false;
    }
    return true;
}

void nyx_watchdog_trigger_failsafe(NyxWatchdog* wd) {
    if (!wd) return;

    switch (wd->on_violation) {
        case NYX_FAILSAFE_REWIND:
            if (wd->arena) {
                /* Zero-cost region rewind: rollback volatile allocations instantly */
                arena_rewind(wd->arena, wd->checkpoint);
            }
            break;

        case NYX_FAILSAFE_HOLD_ACTUATORS:
            /* In real flight silicon: assert hold on CAN/UART flight actuator lines */
            break;

        case NYX_FAILSAFE_BALLISTIC_DESCENT:
            /* In real flight silicon: trigger pyrotechnic ballistic parachute */
            break;
    }
}

bool nyx_watchdog_exit(NyxWatchdog* wd) {
    if (!wd || !wd->active) return true;

    uint64_t now = nyx_read_hw_cycles();
    uint64_t elapsed = now - wd->start_cycles;
    wd->active = false;

    if (elapsed > wd->max_cycles) {
        wd->violations_tripped++;
        nyx_watchdog_trigger_failsafe(wd);
        return false; /* Budget exceeded */
    }

    return true; /* Succeeded within cycle budget */
}
