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

typedef struct {
    int64_t seed;
    NyxVec encoding_map;
    NyxVec decoding_map;
    int64_t key_xor;
} VmOpcodeTable;

typedef struct {
    int64_t opcode;
    int64_t r_dest;
    int64_t r_src;
    int64_t imm32;
} VmInstruction;

typedef struct {
    NyxVec regs;
    int64_t ip;
    int64_t flags;
    NyxVec stack;
    int64_t sp;
    int is_halted;
    int tamper_detected;
} VmRuntimeState;

VmOpcodeTable VmOpcodeTable_new(int64_t);
int64_t VmOpcodeTable_encode_op(VmOpcodeTable*, int64_t);
int64_t VmOpcodeTable_decode_op(VmOpcodeTable*, int64_t);
VmRuntimeState VmRuntimeState_new();
int64_t VmRuntimeState_execute(VmRuntimeState*, NyxVec*, VmOpcodeTable*);
// module vm_obfuscator
// import prelude::*
// import crypto::*
// import sec::*
VmOpcodeTable VmOpcodeTable_new(int64_t seed) {
NyxVec enc = rt_vec_new(sizeof(int64_t));
NyxVec dec = rt_vec_new(sizeof(int64_t));
int64_t i = 0LL;
while ((i < 256LL)) {
rt_vec_push(&enc, ({ int64_t _tmp_0 = (int64_t)((int64_t)(i)); &_tmp_0; }));
rt_vec_push(&dec, ({ int64_t _tmp_1 = (int64_t)((int64_t)(i)); &_tmp_1; }));
i = (i + 1LL);
}
;
int64_t state = (seed ^ (int64_t)(6523356418270964511LL));
int64_t j = 255LL;
while ((j > 0LL)) {
state = ((state * (int64_t)(6364136223846793005LL)) + (int64_t)(1442695040888963407LL));
int64_t target = (int64_t)((state % ((int64_t)(j) + 1LL)));
int64_t tmp = (*(int64_t*)rt_vec_get(enc, j));
(enc[j] = (*(int64_t*)rt_vec_get(enc, target)));
(enc[target] = tmp);
j = (j - 1LL);
}
;
int64_t k = 0LL;
while ((k < 256LL)) {
(dec[(int64_t)((*(int64_t*)rt_vec_get(enc, k)))] = (int64_t)(k));
k = (k + 1LL);
}
;
int64_t key = (int64_t)(((seed >> 24LL) ^ 165LL));
return (VmOpcodeTable){.seed = seed, .encoding_map = enc, .decoding_map = dec, .key_xor = key};
}

int64_t VmOpcodeTable_encode_op(VmOpcodeTable* self, int64_t logical_op) {
return ((*(int64_t*)rt_vec_get((*self).encoding_map, (int64_t)(logical_op))) ^ (*self).key_xor);
}

int64_t VmOpcodeTable_decode_op(VmOpcodeTable* self, int64_t physical_op) {
int64_t unmasked = (physical_op ^ (*self).key_xor);
return (*(int64_t*)rt_vec_get((*self).decoding_map, (int64_t)(unmasked)));
}

VmRuntimeState VmRuntimeState_new() {
NyxVec regs = rt_vec_new(sizeof(int64_t));
for (int64_t _ = 0LL; _ < 16LL; _++) {
rt_vec_push(&regs, ({ int64_t _tmp_2 = (int64_t)((int64_t)(0LL)); &_tmp_2; }));
}
;
NyxVec stack = rt_vec_new(sizeof(int64_t));
for (int64_t _ = 0LL; _ < 256LL; _++) {
rt_vec_push(&stack, ({ int64_t _tmp_3 = (int64_t)((int64_t)(0LL)); &_tmp_3; }));
}
;
return (VmRuntimeState){.regs = regs, .ip = 0LL, .flags = 0LL, .stack = stack, .sp = 0LL, .is_halted = 0, .tamper_detected = 0};
}

int64_t VmRuntimeState_execute(VmRuntimeState* self, NyxVec* bytecode, VmOpcodeTable* table) {
int64_t len = bytecode->len;
(self->ip = 0LL);
(self->is_halted = 0);
while ((((*self).ip < len) && (!(*self).is_halted))) {
({ if ((((*self).ip + 6LL) > len)) {
break;
}
});
int64_t raw_op = (*(int64_t*)rt_vec_get(*bytecode, (*self).ip));
int64_t logical_op = VmOpcodeTable_decode_op(table, raw_op);
int64_t r_dest = (int64_t)(((*(int64_t*)rt_vec_get(*bytecode, ((*self).ip + 1LL))) & 15LL));
int64_t r_src = (int64_t)(((*(int64_t*)rt_vec_get(*bytecode, ((*self).ip + 2LL))) & 15LL));
int64_t imm = ((((int64_t)((*(int64_t*)rt_vec_get(*bytecode, ((*self).ip + 3LL)))) | ((int64_t)((*(int64_t*)rt_vec_get(*bytecode, ((*self).ip + 4LL)))) << 8LL)) | ((int64_t)((*(int64_t*)rt_vec_get(*bytecode, ((*self).ip + 5LL)))) << 16LL)) | ((int64_t)((*(int64_t*)rt_vec_get(*bytecode, ((*self).ip + 6LL)))) << 24LL));
(self->ip = ((*self).ip + 7LL));
({ int64_t __if_res_6; if ((logical_op == VOP_NOP)) {
} else if ((logical_op == VOP_LOAD_IMM)) {
__if_res_6 = ((*self).regs[r_dest] = (int64_t)(imm));
} else if ((logical_op == VOP_MOV)) {
__if_res_6 = ((*self).regs[r_dest] = (*(int64_t*)rt_vec_get((*self).regs, r_src)));
} else if ((logical_op == VOP_ADD_MBA)) {
int64_t a = (*(int64_t*)rt_vec_get((*self).regs, r_dest));
int64_t b = (*(int64_t*)rt_vec_get((*self).regs, r_src));
__if_res_6 = ((*self).regs[r_dest] = ((a ^ b) + ((a & b) << 1LL)));
} else if ((logical_op == VOP_SUB_MBA)) {
int64_t a = (*(int64_t*)rt_vec_get((*self).regs, r_dest));
int64_t b = (*(int64_t*)rt_vec_get((*self).regs, r_src));
__if_res_6 = ((*self).regs[r_dest] = (a - b));
} else if ((logical_op == VOP_XOR)) {
__if_res_6 = ((*self).regs[r_dest] = ((*(int64_t*)rt_vec_get((*self).regs, r_dest)) ^ (*(int64_t*)rt_vec_get((*self).regs, r_src))));
} else if ((logical_op == VOP_AND)) {
__if_res_6 = ((*self).regs[r_dest] = ((*(int64_t*)rt_vec_get((*self).regs, r_dest)) & (*(int64_t*)rt_vec_get((*self).regs, r_src))));
} else if ((logical_op == VOP_OR)) {
__if_res_6 = ((*self).regs[r_dest] = ((*(int64_t*)rt_vec_get((*self).regs, r_dest)) | (*(int64_t*)rt_vec_get((*self).regs, r_src))));
} else if ((logical_op == VOP_SHL)) {
__if_res_6 = ((*self).regs[r_dest] = ((*(int64_t*)rt_vec_get((*self).regs, r_dest)) << (imm & 63LL)));
} else if ((logical_op == VOP_SHR)) {
__if_res_6 = ((*self).regs[r_dest] = ((*(int64_t*)rt_vec_get((*self).regs, r_dest)) >> (imm & 63LL)));
} else if ((logical_op == VOP_CMP)) {
int64_t diff = ((int64_t)((*(int64_t*)rt_vec_get((*self).regs, r_dest))) - (int64_t)((*(int64_t*)rt_vec_get((*self).regs, r_src))));
__if_res_6 = (self->flags = ({ int64_t __if_res_7; if ((diff == 0LL)) {
__if_res_7 = 1LL;
} else if ((diff < 0LL)) {
__if_res_7 = 2LL;
} else {
__if_res_7 = 0LL;
}
__if_res_7; }));
} else if ((logical_op == VOP_JMP)) {
__if_res_6 = (self->ip = (int64_t)(imm));
} else if ((logical_op == VOP_JZ)) {
({ if ((((*self).flags & 1LL) != 0LL)) {
(self->ip = (int64_t)(imm));
}
});
} else if ((logical_op == VOP_JNZ)) {
({ if ((((*self).flags & 1LL) == 0LL)) {
(self->ip = (int64_t)(imm));
}
});
} else if ((logical_op == VOP_PUSH)) {
({ if (((*self).sp < 255LL)) {
((*self).stack[(*self).sp] = (*(int64_t*)rt_vec_get((*self).regs, r_src)));
(self->sp = ((*self).sp + 1LL));
}
});
} else if ((logical_op == VOP_POP)) {
({ if (((*self).sp > 0LL)) {
(self->sp = ((*self).sp - 1LL));
((*self).regs[r_dest] = (*(int64_t*)rt_vec_get((*self).stack, (*self).sp)));
}
});
} else if ((logical_op == VOP_TAMPER_CHK)) {
int64_t canary = ((*(int64_t*)rt_vec_get((*self).regs, r_src)) ^ (int64_t)(-2401053089206453570LL));
({ if ((canary != (int64_t)(imm))) {
(self->tamper_detected = 1);
((*self).regs[0LL] = 0LL);
(self->is_halted = 1);
}
});
} else if ((logical_op == VOP_RET)) {
__if_res_6 = (self->is_halted = 1);
} else {
__if_res_6 = (self->is_halted = 1);
}
__if_res_6; });
}
;
return (*(int64_t*)rt_vec_get((*self).regs, 0LL));
}

