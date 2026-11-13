#ifndef NYX_MLIR_BACKEND_H
#define NYX_MLIR_BACKEND_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "rt_arena.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NYX_REGION_ALLOC,
    NYX_REGION_FREE,
    NYX_ASYNC,
    NYX_AWAIT,
    NYX_TYPE_CHECK,
    NYX_STANDARD,
    /* ── High-Performance Hardware Vector Ops (EuroLLVM 2024 Deep Pipeline) ── */
    NYX_OP_VECTOR_VP_ADD,         /* llvm.intr.vp.fadd / llvm.intr.vp.add */
    NYX_OP_VECTOR_VP_MUL,         /* llvm.intr.vp.fmul / llvm.intr.vp.mul */
    NYX_OP_VECTOR_VP_FMA,         /* llvm.intr.vp.fma (Fused Multiply-Add) */
    NYX_OP_VECTOR_VLA_SVE,        /* Scalable vector<[N]xf32> */
    NYX_OP_VECTOR_REGION_SCRATCH, /* Zero-GC Region-Bound Vector Scratchpad */
    NYX_OP_TRANSFORM_SCHEDULE     /* Declarative MLIR Transform Dialect Sequence */
} NyxOpKind;

typedef struct {
    NyxOpKind kind;
    void* operand;
    int64_t size;
    char* type_name;
    int32_t vector_lanes;         /* Fixed (e.g. 8, 16) or scalable base */
    bool is_scalable;             /* VLA: vector<[N]xT> for RVV / ARM SVE */
    uint32_t explicit_vl;         /* Dynamic EVL (Explicit Vector Length) mask */
} NyxOp;

typedef struct {
    NyxOp* ops;
    int count;
    int capacity;
} NyxPass;

typedef struct {
    void* mlir_context;
    void* mlir_module;
    void* mlir_builder;
    NyxPass* passes;
    int pass_count;
    bool optimized;
    Arena* region_arena;          /* Region-aware vector bufferization anchor */
} NyxMLIRBackend;

typedef struct {
    void* region_header;
    void* bump_ptr;
    void* end_ptr;
    void* next;
    int32_t ref_count;
} RegionHeader;

typedef struct {
    void (*fn)(void*);
    void* arg;
    void* next;
    int32_t state;
    void* result;
} Task;

typedef struct {
    const char* name;
    void* type_id;
    int32_t tag;
} TypeRegistry;

/* Lifecycle */
NyxMLIRBackend* nyx_mlir_create(void);
NyxMLIRBackend* nyx_mlir_create_with_arena(Arena* arena);
void nyx_mlir_destroy(NyxMLIRBackend* backend);

/* Standard Lowering */
void nyx_mlir_lower_region_alloc(NyxMLIRBackend* backend, NyxOp* op);
void nyx_mlir_lower_region_free(NyxMLIRBackend* backend, NyxOp* op);
void nyx_mlir_lower_async(NyxMLIRBackend* backend, NyxOp* op);
void nyx_mlir_lower_await(NyxMLIRBackend* backend, NyxOp* op);
void nyx_mlir_lower_type_check(NyxMLIRBackend* backend, NyxOp* op);
void nyx_mlir_optimize(NyxMLIRBackend* backend);
void nyx_mlir_to_llvm(NyxMLIRBackend* backend);
void nyx_mlir_emit_c(NyxMLIRBackend* backend, char* output, int output_size);

/* ── Deep Hardware Vector Lowering (EuroLLVM 2024 Transform Pipeline) ── */
/* Vector-Predicated (VP) FMA evaluation with dynamic EVL mask */
void nyx_mlir_vector_vp_fma_f32(float* dest, const float* a, const float* b, const float* c, 
                                uint32_t count, uint32_t vl_limit);

/* Region-Aware Vector Scratchpad Alloc (Zero-GC, 64-byte cache aligned) */
float* nyx_mlir_alloc_vector_scratchpad(NyxMLIRBackend* backend, size_t count);

/* Emits Declarative MLIR Transform Dialect Script */
void nyx_mlir_emit_transform_schedule(const char* target_arch, uint32_t tile_size, 
                                      char* out_buf, size_t out_buf_size);

#ifdef __cplusplus
}
#endif

#endif /* NYX_MLIR_BACKEND_H */
