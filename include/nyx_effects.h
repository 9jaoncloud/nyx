/**
 * @file nyx_effects.h
 * @brief Nyx Algebraic Effect Handlers & Delimited Fiber Subsystem
 * @author Simeon Bala (9jaoncloud@gmail.com)
 * @copyright (c) 2026 9jaonCloud Engineering. All Rights Reserved.
 *
 * Provides zero-allocation algebraic effects for async fibers, multi-agent interrupts,
 * generator streams, and deterministic failure recovery.
 */

#ifndef NYX_EFFECTS_H
#define NYX_EFFECTS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NYX_MAX_EFFECT_STACK 64

typedef enum {
    NYX_EFFECT_YIELD      = 1,
    NYX_EFFECT_ASYNC_AWAIT = 2,
    NYX_EFFECT_BARGE_IN   = 3,
    NYX_EFFECT_SWARM_MSG  = 4,
    NYX_EFFECT_FAIL_RECOVER = 5
} nyx_effect_type_t;

typedef struct {
    nyx_effect_type_t type;
    uint64_t payload_u64;
    void* payload_ptr;
    bool is_handled;
} nyx_effect_signal_t;

typedef struct {
    uint32_t fiber_id;
    void* stack_ptr;
    size_t stack_size;
    bool is_suspended;
    nyx_effect_signal_t current_effect;
} nyx_fiber_frame_t;

typedef struct {
    nyx_fiber_frame_t stack[NYX_MAX_EFFECT_STACK];
    size_t depth;
    bool is_active;
} nyx_effect_context_t;

int nyx_effect_init(nyx_effect_context_t* ctx);
int nyx_effect_perform(nyx_effect_context_t* ctx, nyx_effect_type_t type, uint64_t payload);
int nyx_effect_handle(nyx_effect_context_t* ctx, nyx_effect_signal_t* out_signal);
int nyx_effect_resume(nyx_effect_context_t* ctx, uint64_t resume_value);

#ifdef __cplusplus
}
#endif

#endif /* NYX_EFFECTS_H */
