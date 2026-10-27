#ifndef NYX_MLIR_BACKEND_H
#define NYX_MLIR_BACKEND_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    NYX_REGION_ALLOC,
    NYX_REGION_FREE,
    NYX_ASYNC,
    NYX_AWAIT,
    NYX_TYPE_CHECK,
    NYX_STANDARD,
} NyxOpKind;

typedef struct {
    NyxOpKind kind;
    void* operand;
    int64_t size;
    char* type_name;
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

NyxMLIRBackend* nyx_mlir_create(void);
void nyx_mlir_destroy(NyxMLIRBackend* backend);
void nyx_mlir_lower_region_alloc(NyxMLIRBackend* backend, NyxOp* op);
void nyx_mlir_lower_region_free(NyxMLIRBackend* backend, NyxOp* op);
void nyx_mlir_lower_async(NyxMLIRBackend* backend, NyxOp* op);
void nyx_mlir_lower_await(NyxMLIRBackend* backend, NyxOp* op);
void nyx_mlir_lower_type_check(NyxMLIRBackend* backend, NyxOp* op);
void nyx_mlir_optimize(NyxMLIRBackend* backend);
void nyx_mlir_to_llvm(NyxMLIRBackend* backend);
void nyx_mlir_emit_c(NyxMLIRBackend* backend, char* output, int output_size);

#endif
