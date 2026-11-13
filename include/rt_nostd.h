/*
 * rt_nostd.h — Nyx Freestanding Bare-Metal "no_std" Embedded Microcontroller Engine
 *
 * Implements:
 *  1. Zero Libc / Zero OS Syscall Dependency (Runs on bare silicon / 16KB SRAM)
 *  2. Static Pre-Allocated Region Bump Allocator (No malloc/free heap)
 *  3. Memory-Mapped I/O (MMIO) Volatile Register Bit-Banging (GPIO, UART, PWM)
 *  4. Hardware Interrupt Vector Table & SysTick Timer Dispatcher
 */

#ifndef RT_NOSTD_H
#define RT_NOSTD_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EMBEDDED_SRAM_SIZE 16384 /* 16 KB Static SRAM pool */

typedef struct {
    uint8_t memory_pool[EMBEDDED_SRAM_SIZE];
    size_t offset;
    size_t high_watermark;
} NyxEmbeddedArena;

/* Freestanding Arena Allocator (0 heap malloc calls) */
void nyx_embedded_arena_init(NyxEmbeddedArena* arena);
void* nyx_embedded_arena_alloc(NyxEmbeddedArena* arena, size_t size, size_t align);
void nyx_embedded_arena_reset(NyxEmbeddedArena* arena);
size_t nyx_embedded_arena_used(const NyxEmbeddedArena* arena);

/* Direct Memory-Mapped I/O (MMIO) Hardware Register Abstraction */
typedef struct {
    volatile uint32_t MODER;   /* Mode Register (Input/Output/Alternate) */
    volatile uint32_t OTYPER;  /* Output Type */
    volatile uint32_t OSPEEDR; /* Output Speed */
    volatile uint32_t ODR;     /* Output Data Register (Pins 0..15) */
    volatile uint32_t IDR;     /* Input Data Register (Pins 0..15) */
} NyxGpioPortMMIO;

void nyx_mmio_write_pin(NyxGpioPortMMIO* port, uint8_t pin, bool state);
bool nyx_mmio_read_pin(const NyxGpioPortMMIO* port, uint8_t pin);
void nyx_mmio_toggle_pin(NyxGpioPortMMIO* port, uint8_t pin);

/* Interrupt Vector Table */
typedef void (*NyxInterruptHandler)(void);

typedef struct {
    NyxInterruptHandler systick_handler;
    NyxInterruptHandler exti_gpio_handler;
    uint32_t tick_count;
} NyxInterruptVectorTable;

void nyx_isr_init(NyxInterruptVectorTable* ivt);
void nyx_isr_trigger_systick(NyxInterruptVectorTable* ivt);

#ifdef __cplusplus
}
#endif

#endif /* RT_NOSTD_H */
