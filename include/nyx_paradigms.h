/**
 * nyx_paradigms.h — Nyx Native Systems Modernization Core
 * 
 * Implements 6 native systems paradigms designed specifically for Nyx's
 * zero-GC region memory, SIMD vectorization, and sovereign multi-target runtime:
 * 
 *  1. ScopeGuard / Deterministic Resource Scoping (Auto-disposal on region exit)
 *  2. TaggedTemplate / AST-Lowered Parameterized Queries (Zero-allocation SQL/HTML)
 *  3. RegionStream / Coroutine State-Machine Emitters (Zero-allocation data pipelines)
 *  4. TensorView / Multi-Dimensional Strided Slices (Zero-copy ONNX/GEMM slicing)
 *  5. AffineHandle / Move-Only Linear Resource Protection (Single-owner keys & silicon roots)
 *  6. FlatMap / Cache-Contiguous Associative Slices (L1/L2 cache line optimization)
 */

#ifndef NYX_PARADIGMS_H
#define NYX_PARADIGMS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Deterministic ScopeGuard ────────────────────────────── */
typedef void (*NyxDisposeFn)(void* resource);

typedef struct {
    void* resource;
    NyxDisposeFn dispose_fn;
    bool is_disposed;
} NyxScopeGuard;

NyxScopeGuard nyx_guard_create(void* resource, NyxDisposeFn dispose_fn);
void nyx_guard_dismiss(NyxScopeGuard* guard);
void nyx_guard_dispose(NyxScopeGuard* guard);


/* ── 2. Tagged AST Template (Zero-Allocation SQL / HTML) ─────── */
typedef enum {
    NYX_PARAM_INT = 1,
    NYX_PARAM_FLOAT = 2,
    NYX_PARAM_STR = 3
} NyxParamType;

typedef struct {
    NyxParamType type;
    union {
        int64_t i_val;
        double f_val;
        const char* s_val;
    } val;
} NyxParamValue;

typedef struct {
    const char* raw_template;
    uint32_t num_params;
    NyxParamValue params[16];
    char compiled_query[512];
    bool is_sanitized;
} NyxPreparedQuery;

NyxPreparedQuery nyx_template_compile_sql(const char* tmpl, const NyxParamValue* params, uint32_t num_params);


/* ── 3. Zero-Allocation RegionStream Generator ───────────────── */
typedef struct {
    uint32_t state;
    uint32_t current_index;
    uint32_t total_limit;
    int64_t current_value;
    bool is_finished;
} NyxRegionStream;

NyxRegionStream nyx_stream_create(uint32_t total_limit);
bool nyx_stream_next(NyxRegionStream* stream, int64_t* out_val);


/* ── 4. Zero-Copy Multi-Dimensional TensorView ───────────────── */
typedef struct {
    float* data_ptr;       /* Non-owning reference into region or mmap memory */
    uint32_t ndim;
    uint32_t shape[4];     /* Up to 4D tensor (Batch, Channels, Height, Width) */
    uint32_t strides[4];   /* Strides for arbitrary sub-region slicing */
    size_t total_elements;
} NyxTensorView;

NyxTensorView nyx_tensor_view_create(float* data, const uint32_t* shape, uint32_t ndim);
NyxTensorView nyx_tensor_view_slice_2d(const NyxTensorView* parent, uint32_t row_start, uint32_t row_end, uint32_t col_start, uint32_t col_end);
float nyx_tensor_view_get_2d(const NyxTensorView* view, uint32_t row, uint32_t col);


/* ── 5. Linear / Affine Single-Owner Resource Handle ─────────── */
typedef struct {
    uint64_t handle_id;
    void* raw_handle;
    bool is_consumed;      /* Enforces single-use move semantics */
} NyxAffineHandle;

NyxAffineHandle nyx_affine_create(uint64_t id, void* handle);
void* nyx_affine_consume(NyxAffineHandle* handle);


/* ── 6. Contiguous FlatMap (L1/L2 Cache Line Associative Table) ─ */
typedef struct {
    int64_t* keys;         /* Contiguous array of sorted keys */
    int64_t* values;       /* Contiguous array of matching values */
    size_t count;
    size_t capacity;
} NyxFlatMap;

NyxFlatMap nyx_flat_map_create(size_t capacity);
void nyx_flat_map_destroy(NyxFlatMap* map);
bool nyx_flat_map_insert(NyxFlatMap* map, int64_t key, int64_t val);
bool nyx_flat_map_get(const NyxFlatMap* map, int64_t key, int64_t* out_val);

#ifdef __cplusplus
}
#endif

#endif /* NYX_PARADIGMS_H */
