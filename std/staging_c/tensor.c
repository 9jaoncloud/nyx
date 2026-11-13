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
    TensorHandle handle;
    int64_t rows;
    int64_t cols;
} Tensor;

Tensor Tensor_new(int64_t, int64_t);
Tensor Tensor_matmul(Tensor*, Tensor*);
Tensor Tensor_add(Tensor*, Tensor*);
Tensor Tensor_scale(Tensor*, double);
void Tensor_release(Tensor*);
// module tensor
// extern nyx_tensor_create
// extern nyx_tensor_free
// extern nyx_tensor_matmul
// extern nyx_tensor_add
// extern nyx_tensor_scale
Tensor Tensor_new(int64_t rows, int64_t cols) {
return (Tensor){.handle = nyx_tensor_create(rows, cols), .rows = rows, .cols = cols};
}

Tensor Tensor_matmul(Tensor* self, Tensor* other) {
Tensor out = Tensor_new((*self).rows, (*other).cols);
nyx_tensor_matmul((*self).handle, (*other).handle, out.handle);
return out;
}

Tensor Tensor_add(Tensor* self, Tensor* other) {
Tensor out = Tensor_new((*self).rows, (*self).cols);
nyx_tensor_add((*self).handle, (*other).handle, out.handle);
return out;
}

Tensor Tensor_scale(Tensor* self, double s) {
Tensor out = Tensor_new((*self).rows, (*self).cols);
nyx_tensor_scale((*self).handle, s, out.handle);
return out;
}

void Tensor_release(Tensor* self) {
nyx_tensor_free(self.handle);
}

