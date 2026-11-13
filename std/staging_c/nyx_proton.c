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

#define TAG_PeSubsystem_WindowsGui 0
#define TAG_PeSubsystem_WindowsCui 1
#define TAG_PeSubsystem_PosixCui 2
#define TAG_PeSubsystem_Native 3
typedef struct {
    int tag;
    union {
    } data;
} PeSubsystem;

typedef struct {
    int64_t signature;
    int64_t machine_type;
    int64_t number_of_sections;
    int64_t entry_point_rva;
    int64_t image_base;
    PeSubsystem subsystem;
} Pe64Header;

typedef struct {
    int64_t dx_version;
    int is_vkd3d_enabled;
    int64_t active_pipelines;
} DxVulkanPipeline;

typedef struct {
    int64_t active_win32_instances;
    DxVulkanPipeline dx_pipeline;
} MyxProtonBridge;

NyxProtonBridge MyxProtonBridge_new();
NyxResult MyxProtonBridge_load_pe32_executable(MyxProtonBridge*, rt_string_t, void*);
int64_t MyxProtonBridge_translate_directx_command(MyxProtonBridge*, int64_t);
// module nyx_proton
// import std::io
// import std::os::microkernel
void* MyxProtonBridge_new() {
return (NyxProtonBridge){.active_win32_instances = 0LL, .dx_pipeline = (DxVulkanPipeline){.dx_version = 12LL, .is_vkd3d_enabled = 1, .active_pipelines = 0LL}};
}

NyxResult MyxProtonBridge_load_pe32_executable(MyxProtonBridge* self, rt_string_t pe_path, void* token) {
({ if ((!has_permission(token, (CapRights){ .tag = TAG_CapRights_Execute }))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("denied"); _stmp_.data; }); _tmp; });
}
});
(self->active_win32_instances = ((*self).active_win32_instances + 1LL));
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Pe64Header _stmp_ = (Pe64Header){.signature = 17744LL, .machine_type = 34404LL, .number_of_sections = 6LL, .entry_point_rva = 4096LL, .image_base = 5368709120LL, .subsystem = (PeSubsystem){ .tag = TAG_PeSubsystem_WindowsGui }}; &_stmp_; }); _tmp; });
}

int64_t MyxProtonBridge_translate_directx_command(MyxProtonBridge* self, int64_t draw_calls) {
((*self).dx_pipeline.active_pipelines = (((*self).dx_pipeline).active_pipelines + 1LL));
return draw_calls;
}

