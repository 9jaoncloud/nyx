/**
 * @file nyx_effects.c
 * @brief Implementation of Nyx Algebraic Effect Handlers
 * @author Simeon Bala (9jaoncloud@gmail.com)
 * @copyright (c) 2026 9jaonCloud Engineering. All Rights Reserved.
 */

#include "nyx_effects.h"
#include <string.h>

int nyx_effect_init(nyx_effect_context_t* ctx) {
    if (!ctx) return -1;
    memset(ctx, 0, sizeof(nyx_effect_context_t));
    ctx->is_active = true;
    ctx->depth = 0;
    return 0;
}

int nyx_effect_perform(nyx_effect_context_t* ctx, nyx_effect_type_t type, uint64_t payload) {
    if (!ctx || !ctx->is_active) return -1;
    if (ctx->depth >= NYX_MAX_EFFECT_STACK) return -2;

    nyx_fiber_frame_t* frame = &ctx->stack[ctx->depth];
    frame->fiber_id = (uint32_t)ctx->depth + 1;
    frame->is_suspended = true;
    frame->current_effect.type = type;
    frame->current_effect.payload_u64 = payload;
    frame->current_effect.is_handled = false;

    ctx->depth++;
    return 0;
}

int nyx_effect_handle(nyx_effect_context_t* ctx, nyx_effect_signal_t* out_signal) {
    if (!ctx || !ctx->is_active || ctx->depth == 0) return -1;
    if (!out_signal) return -2;

    nyx_fiber_frame_t* top = &ctx->stack[ctx->depth - 1];
    *out_signal = top->current_effect;
    top->current_effect.is_handled = true;
    return 0;
}

int nyx_effect_resume(nyx_effect_context_t* ctx, uint64_t resume_value) {
    if (!ctx || !ctx->is_active || ctx->depth == 0) return -1;

    nyx_fiber_frame_t* top = &ctx->stack[ctx->depth - 1];
    top->is_suspended = false;
    top->current_effect.payload_u64 = resume_value;
    ctx->depth--;
    return 0;
}
