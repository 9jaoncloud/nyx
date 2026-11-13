/*
 * rt_nostd.c — Nyx Freestanding Bare-Metal "no_std" Engine Implementation
 */

#include "rt_nostd.h"
#include <string.h>

void nyx_embedded_arena_init(NyxEmbeddedArena* arena) {
    if (!arena) return;
    arena->offset = 0;
    arena->high_watermark = 0;
    memset(arena->memory_pool, 0, EMBEDDED_SRAM_SIZE);
}

void* nyx_embedded_arena_alloc(NyxEmbeddedArena* arena, size_t size, size_t align) {
    if (!arena || size == 0) return NULL;
    if (align == 0) align = 4;

    size_t current = arena->offset;
    size_t aligned = (current + (align - 1)) & ~(align - 1);

    if (aligned + size > EMBEDDED_SRAM_SIZE) {
        return NULL; /* Out of static SRAM bounds */
    }

    arena->offset = aligned + size;
    if (arena->offset > arena->high_watermark) {
        arena->high_watermark = arena->offset;
    }

    return (void*)&arena->memory_pool[aligned];
}

void nyx_embedded_arena_reset(NyxEmbeddedArena* arena) {
    if (arena) arena->offset = 0;
}

size_t nyx_embedded_arena_used(const NyxEmbeddedArena* arena) {
    return arena ? arena->offset : 0;
}

void nyx_mmio_write_pin(NyxGpioPortMMIO* port, uint8_t pin, bool state) {
    if (!port || pin >= 16) return;
    if (state) {
        port->ODR |= (1u << pin);
    } else {
        port->ODR &= ~(1u << pin);
    }
}

bool nyx_mmio_read_pin(const NyxGpioPortMMIO* port, uint8_t pin) {
    if (!port || pin >= 16) return false;
    return (port->IDR & (1u << pin)) != 0;
}

void nyx_mmio_toggle_pin(NyxGpioPortMMIO* port, uint8_t pin) {
    if (!port || pin >= 16) return;
    port->ODR ^= (1u << pin);
}

void nyx_isr_init(NyxInterruptVectorTable* ivt) {
    if (!ivt) return;
    ivt->systick_handler = NULL;
    ivt->exti_gpio_handler = NULL;
    ivt->tick_count = 0;
}

void nyx_isr_trigger_systick(NyxInterruptVectorTable* ivt) {
    if (!ivt) return;
    ivt->tick_count++;
    if (ivt->systick_handler) {
        ivt->systick_handler();
    }
}
