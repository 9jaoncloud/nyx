#include "runtime/rt_option.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_json.h"
#include <stdint.h>
#include <stdio.h>
#include "runtime/rt_db.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_io.h"
#include "runtime/rt_string.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_map.h"
#include "runtime/rt_async.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "runtime/rt_http.h"
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

#define TAG_BufferUsage_Storage 0
#define TAG_BufferUsage_Uniform 1
#define TAG_BufferUsage_Staging 2
#define TAG_BufferUsage_AccelerationStructure 3
typedef struct {
    int tag;
    union {
    } data;
} BufferUsage;

typedef struct {
    rt_string_t app_name;
    int64_t api_version_major;
    int64_t api_version_minor;
    int validation_enabled;
} VulkanInstance;

typedef struct {
    rt_string_t name;
    int64_t vram_mb;
    int supports_ray_tracing;
    int supports_mesh_shaders;
    int64_t max_workgroup_size;
} PhysicalGpu;

typedef struct {
    rt_string_t name;
    int64_t workgroup_x;
    int64_t workgroup_y;
    int64_t workgroup_z;
} ComputeKernel;

typedef struct {
    double execution_time_ms;
    double bandwidth_gbps;
    int64_t elements_processed;
    double gflops;
} ComputeReport;

VulkanInstance VulkanInstance_new(rt_string_t);
PhysicalGpu query_primary_gpu();
ComputeKernel ComputeKernel_new(rt_string_t, int64_t, int64_t, int64_t);
// module graphics
// import std::string
// import std::option
// import std::vec
void* rt_vk_create_instance(int64_t* p0); // extern
void* rt_vk_query_primary_gpu(); // extern
void* rt_vk_create_swapchain(int64_t p0, int64_t p1); // extern
void* rt_vk_create_buffer(int64_t p0, int64_t p1, int p2); // extern
void rt_vk_free_buffer(void* p0); // extern
void rt_vk_upload_buffer(void* p0, void* p1, int64_t p2); // extern
void rt_vk_read_buffer(void* p0, void* p1, int64_t p2); // extern
void* rt_vk_create_compute_pipeline(int64_t* p0, int64_t p1, int64_t p2, int64_t p3); // extern
VulkanInstance VulkanInstance_new(rt_string_t app_name) {
return (VulkanInstance){.app_name = app_name, .api_version_major = 1LL, .api_version_minor = 3LL, .validation_enabled = 1};
}

PhysicalGpu query_primary_gpu() {
return (PhysicalGpu){.name = int64_t_from(String, rt_string_from("NVIDIA GeForce RTX 4090 (Vulkan 1.3 / SPIR-V 1.6)")), .vram_mb = 24576LL, .supports_ray_tracing = 1, .supports_mesh_shaders = 1, .max_workgroup_size = 1024LL};
}

ComputeKernel ComputeKernel_new(rt_string_t shader_name, int64_t gx, int64_t gy, int64_t gz) {
return (ComputeKernel){.name = shader_name, .workgroup_x = gx, .workgroup_y = gy, .workgroup_z = gz};
}

