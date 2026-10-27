/**
 * nyx_onnx.h — Secure Zero-GC ONNX Model Runtime & Graph Engine for Nyx
 * 
 * Implements native ONNX (Open Neural Network Exchange) tensor parsing,
 * secure protobuf graph descriptor validation, and SIMD-accelerated inference.
 *
 * Security Invariants:
 *  - STRICT bounds checking on tensor dimensions (prevents integer wrap-around)
 *  - ZERO-COPY memory mapping for weights (prevents heap fragmentation)
 *  - Constant-time validation of node attributes (prevents malformed graph injection)
 *  - Sandboxed operator execution (GEMM, MatMul, Relu, Softmax, Add)
 */

#ifndef NYX_ONNX_H
#define NYX_ONNX_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ONNX Data Types (subset matching ONNX spec) */
typedef enum {
    NYX_ONNX_TENSOR_UNDEFINED = 0,
    NYX_ONNX_TENSOR_FLOAT     = 1,  /* IEEE 754 float32 */
    NYX_ONNX_TENSOR_UINT8     = 2,  /* 8-bit unsigned */
    NYX_ONNX_TENSOR_INT8      = 3,  /* 8-bit signed quantized */
    NYX_ONNX_TENSOR_INT32     = 6,  /* 32-bit signed */
    NYX_ONNX_TENSOR_INT64     = 7,  /* 64-bit signed */
    NYX_ONNX_TENSOR_FLOAT16   = 10  /* 16-bit half */
} NyxOnnxDataType;

/* ONNX Operator Types */
typedef enum {
    NYX_ONNX_OP_UNKNOWN = 0,
    NYX_ONNX_OP_GEMM,
    NYX_ONNX_OP_MATMUL,
    NYX_ONNX_OP_RELU,
    NYX_ONNX_OP_SWIGLU,
    NYX_ONNX_OP_SOFTMAX,
    NYX_ONNX_OP_ADD,
    NYX_ONNX_OP_RMSNORM
} NyxOnnxOpType;

/* Secure Tensor Descriptor */
typedef struct {
    char name[64];
    NyxOnnxDataType data_type;
    uint32_t ndim;
    uint32_t dims[8];       /* Max 8 dimensions supported */
    size_t total_elements;  /* Verified against integer overflow */
    float* data;            /* Pointer into region-allocated memory */
    bool is_constant;       /* Weights/biases vs activations */
    bool is_immutable;      /* Locked against mutation via @immutable_region */
} NyxOnnxTensor;

/* Computation Graph Node */
typedef struct {
    char name[64];
    NyxOnnxOpType op_type;
    uint32_t num_inputs;
    char input_names[8][64];
    uint32_t num_outputs;
    char output_names[4][64];
    /* Operator attributes */
    float alpha;            /* GEMM alpha */
    float beta;             /* GEMM beta */
    int trans_a;            /* GEMM transpose A */
    int trans_b;            /* GEMM transpose B */
} NyxOnnxNode;

/* Complete In-Memory Validated ONNX Model Session */
typedef struct {
    char model_name[128];
    int64_t ir_version;
    char producer[64];
    
    /* Validated Tensors (Weights & Inputs) */
    uint32_t num_tensors;
    NyxOnnxTensor* tensors;
    
    /* Validated Execution Nodes */
    uint32_t num_nodes;
    NyxOnnxNode* nodes;
    
    /* Security Verification Audit Flag */
    bool is_verified_secure;
    bool is_sealed_immutable; /* @immutable_region lock for tensor weights */
    size_t memory_bound_bytes;
} NyxOnnxSession;

/* Lifecycle & Execution Functions */
NyxOnnxSession* nyx_onnx_session_create(const char* name, int64_t ir_version);
void nyx_onnx_session_destroy(NyxOnnxSession* session);

/* Hardened Graph Construction & Validation */
bool nyx_onnx_add_tensor(
    NyxOnnxSession* session,
    const char* name,
    NyxOnnxDataType type,
    const uint32_t* dims,
    uint32_t ndim,
    const float* initial_data
);

bool nyx_onnx_add_node(
    NyxOnnxSession* session,
    const char* name,
    NyxOnnxOpType op_type,
    const char** inputs,
    uint32_t num_inputs,
    const char** outputs,
    uint32_t num_outputs
);

/* Formal Model Security Validator: Checks for shape overflows, cyclic dependencies & unbound pointers */
bool nyx_onnx_verify_security(NyxOnnxSession* session, char* out_error, size_t error_len);

/* @immutable_region: Locks constant tensor weight memory pages to prevent Trojan weight tampering */
bool nyx_onnx_seal_immutable_weights(NyxOnnxSession* session);

/* @clamp: Sanitizes incoming input float buffers against NaN, +Inf, -Inf */
bool nyx_onnx_sanitize_input(float* data, size_t len, float min_val, float max_val);

/* Forward Inference Pass */
bool nyx_onnx_forward(
    NyxOnnxSession* session,
    const char* input_name,
    const float* input_data,
    size_t input_len,
    const char* output_name,
    float* out_data,
    size_t out_len
);

#ifdef __cplusplus
}
#endif

#endif /* NYX_ONNX_H */
