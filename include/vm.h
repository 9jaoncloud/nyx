#ifndef NYX_VM_H
#define NYX_VM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define NYX_STACK_MAX 256
#define NYX_FRAMES_MAX 64

typedef enum {
    NYX_VAL_NIL,
    NYX_VAL_BOOL,
    NYX_VAL_NUMBER,
    NYX_VAL_STRING,
    NYX_VAL_OBJ,
    NYX_VAL_ARRAY,
    NYX_VAL_CLOSURE,
} NyxValueType;

typedef struct NyxObject {
    int ref_count;
    NyxValueType type;
    void* data;
    struct NyxObject* next;
} NyxObject;

typedef struct {
    NyxValueType type;
    union {
        bool boolean;
        double number;
        char* string;
        NyxObject* obj;
        struct { uint32_t* items; int length; } array;
        struct { struct NyxClosure* closure; } closure;
    } as;
} NyxValue;

typedef enum {
    OP_CONSTANT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_POP,
    OP_DUP,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_GET_GLOBAL,
    OP_DEFINE_GLOBAL,
    OP_SET_GLOBAL,
    OP_GET_UPVALUE,
    OP_SET_UPVALUE,
    OP_GET_PROPERTY,
    OP_SET_PROPERTY,
    OP_GET_INDEX,
    OP_SET_INDEX,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MODULO,
    OP_NOT,
    OP_NEGATE,
    OP_PRINT,
    OP_PRINTLN,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_JUMP_IF_TRUE,
    OP_LOOP,
    OP_CALL,
    OP_CLOSURE,
    OP_CLOSE_UPVALUE,
    OP_RETURN,
    OP_CLASS,
    OP_METHOD,
    OP_INVOKE,
    OP_INHERIT,
    OP_SUPER,
    OP_REGION_ALLOC,
    OP_REGION_FREE,
    OP_ARC_RETAIN,
    OP_ARC_RELEASE,
    OP_AWAIT,
    OP_TYPEOF,
    OP_LEN,
    OP_STRING_CONCAT,
    OP_ASSERT,
} NyxOpCode;

typedef struct {
    uint8_t* code;
    int* lines;
    int count;
    int capacity;
    NyxValue* constants;
    int constant_count;
    int constant_capacity;
} NyxChunk;

typedef struct {
    NyxChunk* chunk;
    uint8_t* ip;
    NyxValue* slots;
    int slot_count;
    uint8_t** open_upvalues;
    int upvalue_count;
} NyxCallFrame;

typedef struct NyxClosure {
    NyxChunk* chunk;
    NyxValue* upvalues;
    int upvalue_count;
} NyxClosure;

typedef struct NyxVM {
    NyxChunk* chunk;
    uint8_t* ip;
    NyxValue stack[NYX_STACK_MAX];
    NyxValue* stack_top;
    NyxCallFrame frames[NYX_FRAMES_MAX];
    int frame_count;
    NyxObject* objects;
    struct { char* key; NyxValue value; } globals[256];
    int global_count;
    NyxObject* open_upvalues;
    bool had_error;
    char error_msg[256];
} NyxVM;

typedef enum {
    NYX_INTERPRET_OK,
    NYX_INTERPRET_COMPILE_ERROR,
    NYX_INTERPRET_RUNTIME_ERROR,
} NyxInterpretResult;

void nyx_vm_init(NyxVM* vm);
void nyx_vm_free(NyxVM* vm);
NyxInterpretResult nyx_vm_interpret(NyxVM* vm, const char* source);
void nyx_vm_push(NyxVM* vm, NyxValue value);
NyxValue nyx_vm_pop(NyxVM* vm);
NyxValue nyx_vm_peek(NyxVM* vm, int distance);
void nyx_chunk_init(NyxChunk* chunk);
void nyx_chunk_free(NyxChunk* chunk);
int nyx_chunk_add_constant(NyxChunk* chunk, NyxValue value);
NyxValue nyx_value_nil(void);
NyxValue nyx_value_bool(bool b);
NyxValue nyx_value_number(double n);
NyxValue nyx_value_string(const char* s);
NyxObject* nyx_object_new(NyxVM* vm, NyxValueType type, void* data);
void nyx_object_retain(NyxObject* obj);
void nyx_object_release(NyxVM* vm, NyxObject* obj);
NyxChunk* nyx_compile_source(const char* source);
void nyx_chunk_disassemble(NyxChunk* chunk, const char* name);

#endif
