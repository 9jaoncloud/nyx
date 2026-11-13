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
#include "runtime/rt_atomic.h"
#include "runtime/nyx_generational.h"
#include "include/nyx_multidialect.h"
/* Odin-inspired Native Execution Context */
typedef struct NyxContext {
    void* nursery;
    void* temp_nursery;
    uint64_t thread_id;
    void (*logger)(const char* msg);
    void* user_data;
} NyxContext;
#if defined(_MSC_VER)
__declspec(thread) NyxContext nyx_current_context = {0};
#else
__thread NyxContext nyx_current_context = {0};
#endif
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

void nyx_main();

#line 1 "esp32_end_to_end_demo.nyx"
// module examples.esp32_end_to_end_demo
// import std::io
// import std::embedded::esp32
// import std::net::lora
void nyx_main() {
Arena* __arena = arena_create();
printf("%s\n", rt_string_from("=== Nyx ESP32 Bare-Metal Native Execution ===").data);
int64_t core = int64_t_current_core(Esp32DualCoreDispatcher);
({
int64_t __match_result_0;
int64_t __match_val_0 = core;
if (__match_val_0.tag == TAG_CpuCore_Core0_Protocol) {
printf("%s\n", rt_string_from("[Boot] Running on Core 0 (Protocol & Mesh Controller)").data);
}
 else if (__match_val_0.tag == TAG_CpuCore_Core1_Compute) {
printf("%s\n", rt_string_from("[Boot] Running on Core 1 (DSP / Real-Time Loop)").data);
}
__match_result_0;
});
Esp32Gpio status_led = int64_t_new(Esp32Gpio, 2LL);
Esp32Gpio_set_direction_output(&(status_led));
printf("%s\n", rt_string_from("[GPIO] Pin 2 configured as HIGH-SPEED DIGITAL OUTPUT").data);
Esp32Gpio_write_high(&(status_led));
printf("%s\n", rt_string_from("[GPIO] LED State: HIGH (3.3V)").data);
Esp32Gpio_write_low(&(status_led));
printf("%s\n", rt_string_from("[GPIO] LED State: LOW (0.0V)").data);
int64_t img_hdr = int64_t_new_standard(Esp32ImageHeader, 1074266112LL);
printf("%s\n", rt_string_from("[Toolchain] Espressif Binary Header Formatted:").data);
printf("%s\n", rt_string_concat(rt_string_from("  - Magic: 0x"), (rt_string_t)(img_hdr.magic)).data);
printf("%s\n", rt_string_concat(rt_string_from("  - Segment Count: "), (rt_string_t)(img_hdr.segment_count)).data);
printf("%s\n", rt_string_from("  - Entry Point: 0x40080000 (IRAM Reset Vector)").data);
printf("%s\n", rt_string_from("\n[SUCCESS] ESP32 end-to-end hardware loop executed in 0 bytes dynamic heap!").data);
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
