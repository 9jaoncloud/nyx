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
    NyxVec data;
    NyxVec shape;
    NyxOption grad;
    int requires_grad;
} Tensor;

typedef struct {
    NyxVec tape;
} Autograd;

typedef struct {
    int64_t tensor_idx;
    rt_string_t grad_fn;
    NyxVec inputs;
} GradEntry;

typedef struct {
    double lr;
    double momentum;
    NyxVec velocity;
} SGD;

typedef struct {
    double lr;
    double beta1;
    double beta2;
    double epsilon;
    NyxVec m;
    NyxVec v;
    int64_t t;
} Adam;

typedef struct {
    double initial_lr;
    int64_t step_size;
    double gamma;
} LRcheduler;

typedef struct {
    Tensor weight;
    Tensor bias;
} Linear;

typedef struct {
    Tensor weight;
    Tensor bias;
    double eps;
} LayerNorm;

typedef struct {
    Tensor weight;
} Embedding;

typedef struct {
    double rate;
} Dropout;

typedef struct {
    int64_t embed_dim;
    int64_t num_heads;
    Linear q_proj;
    Linear k_proj;
    Linear v_proj;
    Linear out_proj;
} MultiheadAttention;

typedef struct {
    MultiheadAttention self_attn;
    Linear fc1;
    Linear fc2;
    LayerNorm norm1;
    LayerNorm norm2;
} TransformerBlock;

typedef struct {
    int64_t epochs;
    int64_t batch_size;
    double learning_rate;
} Trainer;

typedef struct {
    Tensor data;
    Tensor targets;
    int64_t batch_size;
    int shuffle;
} DataLoader;

Tensor Tensor_new(NyxVec, NyxVec);
Tensor Tensor_zeros(NyxVec);
Tensor Tensor_ones(NyxVec);
Tensor Tensor_from_slice(NyxVec);
Tensor Tensor_scalar(double);
Tensor Tensor_rand(NyxVec);
Tensor Tensor_randn(NyxVec);
void Tensor_requires_grad_(Tensor*, int);
NyxVec Tensor_shape(Tensor*);
int64_t Tensor_ndim(Tensor*);
int64_t Tensor_size(Tensor*);
double Tensor_item(Tensor*);
rt_string_t Tensor_to_string(Tensor*);
Tensor add(Tensor*, Tensor*);
Tensor sub(Tensor*, Tensor*);
Tensor mul(Tensor*, Tensor*);
Tensor div(Tensor*, Tensor*);
Tensor matmul(Tensor*, Tensor*);
Tensor dot(Tensor*, Tensor*);
Tensor sum(Tensor*);
Tensor mean(Tensor*);
Tensor nyx_max(Tensor*);
Tensor nyx_min(Tensor*);
Tensor nyx_abs(Tensor*);
Tensor relu(Tensor*);
Tensor sigmoid(Tensor*);
Tensor tanh(Tensor*);
Tensor softmax(Tensor*);
Tensor log_softmax(Tensor*);
Tensor exp(Tensor*);
Tensor log(Tensor*);
Tensor pow(Tensor*, double);
Tensor sqrt(Tensor*);
Tensor clamp(Tensor*, double, double);
Tensor transpose(Tensor*);
Tensor reshape(Tensor*, NyxVec);
Tensor flatten(Tensor*);
Tensor cat(NyxVec, int64_t);
Tensor stack(NyxVec);
Autograd Autograd_new();
Tensor mse_loss(Tensor*, Tensor*);
Tensor cross_entropy_loss(Tensor*, Tensor*);
Tensor binary_cross_entropy(Tensor*, Tensor*);
Tensor nll_loss(Tensor*, Tensor*);
SGD SGD_new(double);
SGD SGD_with_momentum(double, double);
void SGD_step(SGD*, NyxVec*);
Adam Adam_new(double);
Adam Adam_with_betas(double, double, double);
void Adam_step(Adam*, NyxVec*);
LRcheduler LRcheduler_step_lr(double, int64_t, double);
double LRcheduler_get_lr(LRcheduler*, int64_t);
Linear Linear_new(int64_t, int64_t);
Tensor Linear_forward(Linear*, Tensor*);
LayerNorm LayerNorm_new(int64_t);
Tensor LayerNorm_forward(LayerNorm*, Tensor*);
Embedding Embedding_new(int64_t, int64_t);
Tensor Embedding_forward(Embedding*, Tensor*);
Dropout Dropout_new(double);
Tensor Dropout_forward(Dropout*, Tensor*);
MultiheadAttention MultiheadAttention_new(int64_t, int64_t);
Tensor MultiheadAttention_forward(MultiheadAttention*, Tensor*, Tensor*, Tensor*);
TransformerBlock TransformerBlock_new(int64_t, int64_t, int64_t);
Tensor TransformerBlock_forward(TransformerBlock*, Tensor*);
Trainer Trainer_new(int64_t, int64_t, double);
NyxVec Trainer_train(Trainer*, Tensor (*)(Tensor*), Tensor*, Tensor*);
DataLoader DataLoader_new(Tensor, Tensor, int64_t);
DataLoader DataLoader_with_shuffle(Tensor, Tensor, int64_t, int);
int64_t DataLoader_len(DataLoader*);
NyxVec DataLoader_batches(DataLoader*);
Tensor one_hot(Tensor*, int64_t);
Tensor arange(double, double, double);
Tensor linspace(double, double, int64_t);
// module ml
Tensor Tensor_new(NyxVec data, NyxVec shape) {
return (Tensor){.data = data, .shape = shape, .grad = rt_option_none(), .requires_grad = 0};
}

Tensor Tensor_zeros(NyxVec shape) {
int64_t size = 1LL;
for (int64_t _dim = 0; _dim < shape.len; _dim++) {
    int64_t dim = *(int64_t*)rt_vec_get(shape, _dim);
size = (size * dim);
}
;
int64_t data = Vec_with_capacity(size);
return (Tensor){.data = data, .shape = shape, .grad = rt_option_none(), .requires_grad = 0};
}

Tensor Tensor_ones(NyxVec shape) {
int64_t size = 1LL;
for (int64_t _dim = 0; _dim < shape.len; _dim++) {
    int64_t dim = *(int64_t*)rt_vec_get(shape, _dim);
size = (size * dim);
}
;
int64_t data = Vec_with_capacity(size);
for (int64_t _ = 0LL; _ < size; _++) {
rt_vec_push(&data, ({ double _tmp_0 = (int64_t)(1); &_tmp_0; }));
}
;
return (Tensor){.data = data, .shape = shape, .grad = rt_option_none(), .requires_grad = 0};
}

Tensor Tensor_from_slice(NyxVec data) {
NyxVec shape = ({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve1_0 = data.len; rt_vec_push(&_v, &_ve1_0); _v; });
return (Tensor){.data = data, .shape = shape, .grad = rt_option_none(), .requires_grad = 0};
}

Tensor Tensor_scalar(double value) {
NyxVec data = rt_vec_new(sizeof(int64_t));
rt_vec_push(&data, ({ double _tmp_1 = (int64_t)(value); &_tmp_1; }));
return (Tensor){.data = data, .shape = ({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve2_0 = 1LL; rt_vec_push(&_v, &_ve2_0); _v; }), .grad = rt_option_none(), .requires_grad = 0};
}

Tensor Tensor_rand(NyxVec shape) {
int64_t size = 1LL;
for (int64_t _dim = 0; _dim < shape.len; _dim++) {
    int64_t dim = *(int64_t*)rt_vec_get(shape, _dim);
size = (size * dim);
}
;
int64_t data = Vec_with_capacity(size);
for (int64_t _ = 0LL; _ < size; _++) {
rt_vec_push(&data, ({ double _tmp_2 = (int64_t)(0.5); &_tmp_2; }));
}
;
return (Tensor){.data = data, .shape = shape, .grad = rt_option_none(), .requires_grad = 0};
}

Tensor Tensor_randn(NyxVec shape) {
return Tensor_rand(shape);
}

void Tensor_requires_grad_(Tensor* self, int req) {
(self->requires_grad = req);
}

NyxVec Tensor_shape(Tensor* self) {
return rt_vec_clone((*self).shape);
}

int64_t Tensor_ndim(Tensor* self) {
return (*self).shape.len;
}

int64_t Tensor_size(Tensor* self) {
int64_t s = 1LL;
for (int64_t _d = 0; _d < (*self).shape.len; _d++) {
    int64_t d = *(int64_t*)rt_vec_get((*self).shape, _d);
s = (s * d);
}
;
return s;
}

double Tensor_item(Tensor* self) {
return (*(int64_t*)rt_vec_get((*self).data, 0LL));
}

rt_string_t Tensor_to_string(Tensor* self) {
rt_string_t result = rt_string_from("");
result = rt_string_concat(result, rt_string_from("Tensor(shape=["));
int first = 1;
for (int64_t _d = 0; _d < (*self).shape.len; _d++) {
    int64_t d = *(int64_t*)rt_vec_get((*self).shape, _d);
({ if ((!first)) {
result = rt_string_concat(result, rt_string_from(", "));
}
});
result = rt_string_concat(result, ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)d); _buf.ref_count = 1; _buf; }));
first = 0;
}
;
result = rt_string_concat(result, rt_string_from("])"));
return result;
}

Tensor add(Tensor* a, Tensor* b) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*a).shape));
for (int64_t i = 0LL; i < (*a).data.len; i++) {
(result.data[i] = ((*(int64_t*)rt_vec_get((*a).data, i)) + (*(int64_t*)rt_vec_get((*b).data, i))));
}
;
arena_destroy(__arena);
return result;
}

Tensor sub(Tensor* a, Tensor* b) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*a).shape));
for (int64_t i = 0LL; i < (*a).data.len; i++) {
(result.data[i] = ((*(int64_t*)rt_vec_get((*a).data, i)) - (*(int64_t*)rt_vec_get((*b).data, i))));
}
;
arena_destroy(__arena);
return result;
}

Tensor mul(Tensor* a, Tensor* b) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*a).shape));
for (int64_t i = 0LL; i < (*a).data.len; i++) {
(result.data[i] = ((*(int64_t*)rt_vec_get((*a).data, i)) * (*(int64_t*)rt_vec_get((*b).data, i))));
}
;
arena_destroy(__arena);
return result;
}

Tensor div(Tensor* a, Tensor* b) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*a).shape));
for (int64_t i = 0LL; i < (*a).data.len; i++) {
(result.data[i] = ((*(int64_t*)rt_vec_get((*a).data, i)) / (*(int64_t*)rt_vec_get((*b).data, i))));
}
;
arena_destroy(__arena);
return result;
}

Tensor matmul(Tensor* a, Tensor* b) {
Arena* __arena = arena_create();
int64_t m = (*(int64_t*)rt_vec_get((*a).shape, 0LL));
int64_t k = (*(int64_t*)rt_vec_get((*a).shape, 1LL));
int64_t n = (*(int64_t*)rt_vec_get((*b).shape, 1LL));
Tensor result = Tensor_zeros(({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve4_0 = m; rt_vec_push(&_v, &_ve4_0); int64_t _ve4_1 = n; rt_vec_push(&_v, &_ve4_1); _v; }));
for (int64_t i = 0LL; i < m; i++) {
for (int64_t j = 0LL; j < n; j++) {
double sum = 0;
for (int64_t l = 0LL; l < k; l++) {
sum = (sum + ((*(int64_t*)rt_vec_get((*a).data, ((i * k) + l))) * (*(int64_t*)rt_vec_get((*b).data, ((l * n) + j)))));
}
;
(result.data[((i * n) + j)] = sum);
}
;
}
;
arena_destroy(__arena);
return result;
}

Tensor dot(Tensor* a, Tensor* b) {
Arena* __arena = arena_create();
double sum = 0;
for (int64_t i = 0LL; i < (*a).data.len; i++) {
sum = (sum + ((*(int64_t*)rt_vec_get((*a).data, i)) * (*(int64_t*)rt_vec_get((*b).data, i))));
}
;
arena_destroy(__arena);
return Tensor_scalar(sum);
}

Tensor sum(Tensor* t) {
Arena* __arena = arena_create();
double s = 0;
for (int64_t _v = 0; _v < (*t).data.len; _v++) {
    double v = *(double*)rt_vec_get((*t).data, _v);
s = (s + v);
}
;
arena_destroy(__arena);
return Tensor_scalar(s);
}

Tensor mean(Tensor* t) {
Arena* __arena = arena_create();
Tensor s = sum(t);
arena_destroy(__arena);
return Tensor_scalar((Tensor_item(&(s)) / (double)((*t).data.len)));
}

Tensor nyx_max(Tensor* t) {
Arena* __arena = arena_create();
int64_t m = (*(int64_t*)rt_vec_get((*t).data, 0LL));
for (int64_t _v = 0; _v < (*t).data.len; _v++) {
    double v = *(double*)rt_vec_get((*t).data, _v);
({ if ((v > m)) {
m = v;
}
});
}
;
arena_destroy(__arena);
return Tensor_scalar(m);
}

Tensor nyx_min(Tensor* t) {
Arena* __arena = arena_create();
int64_t m = (*(int64_t*)rt_vec_get((*t).data, 0LL));
for (int64_t _v = 0; _v < (*t).data.len; _v++) {
    double v = *(double*)rt_vec_get((*t).data, _v);
({ if ((v < m)) {
m = v;
}
});
}
;
arena_destroy(__arena);
return Tensor_scalar(m);
}

Tensor nyx_abs(Tensor* t) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*t).shape));
for (int64_t i = 0LL; i < (*t).data.len; i++) {
(result.data[i] = ({ int64_t __if_res_7; if (((*(int64_t*)rt_vec_get((*t).data, i)) < 0)) {
__if_res_7 = (-(*(int64_t*)rt_vec_get((*t).data, i)));
} else {
__if_res_7 = (*(int64_t*)rt_vec_get((*t).data, i));
}
__if_res_7; }));
}
;
arena_destroy(__arena);
return result;
}

Tensor relu(Tensor* t) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*t).shape));
for (int64_t i = 0LL; i < (*t).data.len; i++) {
(result.data[i] = ({ int64_t __if_res_8; if (((*(int64_t*)rt_vec_get((*t).data, i)) > 0)) {
__if_res_8 = (*(int64_t*)rt_vec_get((*t).data, i));
} else {
__if_res_8 = 0;
}
__if_res_8; }));
}
;
arena_destroy(__arena);
return result;
}

Tensor sigmoid(Tensor* t) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*t).shape));
for (int64_t i = 0LL; i < (*t).data.len; i++) {
(result.data[i] = (1 / (1 + int64_t_exp((-(*(int64_t*)rt_vec_get((*t).data, i)))))));
}
;
arena_destroy(__arena);
return result;
}

Tensor tanh(Tensor* t) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*t).shape));
for (int64_t i = 0LL; i < (*t).data.len; i++) {
(result.data[i] = int64_t_tanh((*(int64_t*)rt_vec_get((*t).data, i))));
}
;
arena_destroy(__arena);
return result;
}

Tensor softmax(Tensor* t) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*t).shape));
int64_t max_val = (*(int64_t*)rt_vec_get((*t).data, 0LL));
for (int64_t _v = 0; _v < (*t).data.len; _v++) {
    double v = *(double*)rt_vec_get((*t).data, _v);
({ if ((v > max_val)) {
max_val = v;
}
});
}
;
double sum_exp = 0;
for (int64_t i = 0LL; i < (*t).data.len; i++) {
(result.data[i] = int64_t_exp(((*(int64_t*)rt_vec_get((*t).data, i)) - max_val)));
sum_exp = (sum_exp + (*(int64_t*)rt_vec_get(result.data, i)));
}
;
for (int64_t i = 0LL; i < result.data.len; i++) {
(result.data[i] = ((*(int64_t*)rt_vec_get(result.data, i)) / sum_exp));
}
;
arena_destroy(__arena);
return result;
}

Tensor log_softmax(Tensor* t) {
Arena* __arena = arena_create();
Tensor s = softmax(t);
Tensor result = Tensor_zeros(rt_vec_clone(s.shape));
for (int64_t i = 0LL; i < s.data.len; i++) {
(result.data[i] = int64_t_ln((*(int64_t*)rt_vec_get(s.data, i))));
}
;
arena_destroy(__arena);
return result;
}

Tensor exp(Tensor* t) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*t).shape));
for (int64_t i = 0LL; i < (*t).data.len; i++) {
(result.data[i] = int64_t_exp((*(int64_t*)rt_vec_get((*t).data, i))));
}
;
arena_destroy(__arena);
return result;
}

Tensor log(Tensor* t) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*t).shape));
for (int64_t i = 0LL; i < (*t).data.len; i++) {
(result.data[i] = int64_t_ln((*(int64_t*)rt_vec_get((*t).data, i))));
}
;
arena_destroy(__arena);
return result;
}

Tensor pow(Tensor* t, double n) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*t).shape));
for (int64_t i = 0LL; i < (*t).data.len; i++) {
(result.data[i] = int64_t_powf((*(int64_t*)rt_vec_get((*t).data, i)), n));
}
;
arena_destroy(__arena);
return result;
}

Tensor sqrt(Tensor* t) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*t).shape));
for (int64_t i = 0LL; i < (*t).data.len; i++) {
(result.data[i] = sqrt((*(int64_t*)rt_vec_get((*t).data, i))));
}
;
arena_destroy(__arena);
return result;
}

Tensor clamp(Tensor* t, double min, double max) {
Arena* __arena = arena_create();
Tensor result = Tensor_zeros(rt_vec_clone((*t).shape));
for (int64_t i = 0LL; i < (*t).data.len; i++) {
int64_t v = (*(int64_t*)rt_vec_get((*t).data, i));
(result.data[i] = ({ double __if_res_10; if ((v < min)) {
__if_res_10 = min;
} else if ((v > max)) {
__if_res_10 = max;
} else {
__if_res_10 = v;
}
__if_res_10; }));
}
;
arena_destroy(__arena);
return result;
}

Tensor transpose(Tensor* t) {
Arena* __arena = arena_create();
int64_t rows = (*(int64_t*)rt_vec_get((*t).shape, 0LL));
int64_t cols = (*(int64_t*)rt_vec_get((*t).shape, 1LL));
Tensor result = Tensor_zeros(({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve10_0 = cols; rt_vec_push(&_v, &_ve10_0); int64_t _ve10_1 = rows; rt_vec_push(&_v, &_ve10_1); _v; }));
for (int64_t i = 0LL; i < rows; i++) {
for (int64_t j = 0LL; j < cols; j++) {
(result.data[((j * rows) + i)] = (*(int64_t*)rt_vec_get((*t).data, ((i * cols) + j))));
}
;
}
;
arena_destroy(__arena);
return result;
}

Tensor reshape(Tensor* t, NyxVec new_shape) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (Tensor){.data = rt_vec_clone((*t).data), .shape = new_shape, .grad = rt_option_none(), .requires_grad = (*t).requires_grad};
}

Tensor flatten(Tensor* t) {
Arena* __arena = arena_create();
NyxVec data = rt_vec_new(sizeof(int64_t));
for (int64_t _v = 0; _v < (*t).data.len; _v++) {
    double v = *(double*)rt_vec_get((*t).data, _v);
rt_vec_push(&data, ({ double _tmp_10 = (int64_t)(v); &_tmp_10; }));
}
;
arena_destroy(__arena);
return (Tensor){.data = data, .shape = ({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve11_0 = Tensor_size(t); rt_vec_push(&_v, &_ve11_0); _v; }), .grad = rt_option_none(), .requires_grad = (*t).requires_grad};
}

Tensor cat(NyxVec tensors, int64_t dim) {
Arena* __arena = arena_create();
({ if ((tensors.len == 0LL)) {
return Tensor_zeros(({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve12_0 = 0LL; rt_vec_push(&_v, &_ve12_0); _v; }));
}
});
NyxVec result_data = rt_vec_new(sizeof(int64_t));
NyxVec result_shape = rt_vec_clone(((*(Tensor*)rt_vec_get(tensors, 0LL))).shape);
for (int64_t _t = 0; _t < tensors.len; _t++) {
    Tensor t = *(Tensor*)rt_vec_get(tensors, _t);
for (int64_t _v = 0; _v < t.data.len; _v++) {
    double v = *(double*)rt_vec_get(t.data, _v);
rt_vec_push(&result_data, ({ double _tmp_12 = (int64_t)(v); &_tmp_12; }));
}
;
(result_shape[dim] = ((*(int64_t*)rt_vec_get(result_shape, dim)) + (*(int64_t*)rt_vec_get(t.shape, dim))));
}
;
arena_destroy(__arena);
return (Tensor){.data = result_data, .shape = result_shape, .grad = rt_option_none(), .requires_grad = 0};
}

Tensor stack(NyxVec tensors) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return cat(tensors, 0LL);
}

Autograd Autograd_new() {
return (Autograd){.tape = rt_vec_new(sizeof(int64_t))};
}

Tensor mse_loss(Tensor* pred, Tensor* target) {
Arena* __arena = arena_create();
double loss = 0;
for (int64_t i = 0LL; i < (*pred).data.len; i++) {
int64_t diff = ((*(int64_t*)rt_vec_get((*pred).data, i)) - (*(int64_t*)rt_vec_get((*target).data, i)));
loss = (loss + (diff * diff));
}
;
arena_destroy(__arena);
return Tensor_scalar((loss / (double)((*pred).data.len)));
}

Tensor cross_entropy_loss(Tensor* pred, Tensor* target) {
Arena* __arena = arena_create();
Tensor ls = log_softmax(pred);
double loss = 0;
for (int64_t i = 0LL; i < ls.data.len; i++) {
loss = (loss + (-((*(int64_t*)rt_vec_get((*target).data, i)) * (*(int64_t*)rt_vec_get(ls.data, i)))));
}
;
arena_destroy(__arena);
return Tensor_scalar(loss);
}

Tensor binary_cross_entropy(Tensor* pred, Tensor* target) {
Arena* __arena = arena_create();
double loss = 0;
int64_t eps = (0LL - 7LL);
for (int64_t i = 0LL; i < (*pred).data.len; i++) {
Tensor p = int64_t_clamp((*(int64_t*)rt_vec_get((*pred).data, i)), eps, (1 - eps));
loss = (loss - (((*(int64_t*)rt_vec_get((*target).data, i)) * Tensor_ln(&(p))) + ((1 - (*(int64_t*)rt_vec_get((*target).data, i))) * ln((1 - p)))));
}
;
arena_destroy(__arena);
return Tensor_scalar((loss / (double)((*pred).data.len)));
}

Tensor nll_loss(Tensor* pred, Tensor* target) {
Arena* __arena = arena_create();
double loss = 0;
int64_t batch = (*(int64_t*)rt_vec_get((*pred).shape, 0LL));
int64_t classes = (*(int64_t*)rt_vec_get((*pred).shape, 1LL));
for (int64_t i = 0LL; i < batch; i++) {
int64_t cls = (int64_t)((*(int64_t*)rt_vec_get((*target).data, i)));
loss = (loss + (-(*(int64_t*)rt_vec_get((*pred).data, ((i * classes) + cls)))));
}
;
arena_destroy(__arena);
return Tensor_scalar((loss / (double)(batch)));
}

SGD SGD_new(double lr) {
return (SGD){.lr = lr, .momentum = 0, .velocity = rt_vec_new(sizeof(int64_t))};
}

SGD SGD_with_momentum(double lr, double momentum) {
return (SGD){.lr = lr, .momentum = momentum, .velocity = rt_vec_new(sizeof(int64_t))};
}

void SGD_step(SGD* self, NyxVec* params) {
}

Adam Adam_new(double lr) {
return (Adam){.lr = lr, .beta1 = 0.9, .beta2 = 0.999, .epsilon = (0LL - 8LL), .m = rt_vec_new(sizeof(int64_t)), .v = rt_vec_new(sizeof(int64_t)), .t = 0LL};
}

Adam Adam_with_betas(double lr, double b1, double b2) {
return (Adam){.lr = lr, .beta1 = b1, .beta2 = b2, .epsilon = (0LL - 8LL), .m = rt_vec_new(sizeof(int64_t)), .v = rt_vec_new(sizeof(int64_t)), .t = 0LL};
}

void Adam_step(Adam* self, NyxVec* params) {
(self->t = ((*self).t + 1LL));
}

LRcheduler LRcheduler_step_lr(double initial_lr, int64_t step_size, double gamma) {
return (LRcheduler){.initial_lr = initial_lr, .step_size = step_size, .gamma = gamma};
}

double LRcheduler_get_lr(LRcheduler* self, int64_t epoch) {
double factor = (double)((epoch / (*self).step_size));
return ((*self).initial_lr * powf((*self).gamma, factor));
}

Linear Linear_new(int64_t in_features, int64_t out_features) {
Tensor w = Tensor_rand(({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve13_0 = out_features; rt_vec_push(&_v, &_ve13_0); int64_t _ve13_1 = in_features; rt_vec_push(&_v, &_ve13_1); _v; }));
Tensor b = Tensor_zeros(({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve13_0 = out_features; rt_vec_push(&_v, &_ve13_0); _v; }));
return (Linear){.weight = w, .bias = b};
}

Tensor Linear_forward(Linear* self, Tensor* input) {
Tensor out = matmul(input, ({ Tensor _atmp_1 = (&((*self).weight)); &_atmp_1; }));
return add(({ Tensor _atmp_0 = (&(out)); &_atmp_0; }), ({ Tensor _atmp_1 = (&((*self).bias)); &_atmp_1; }));
}

LayerNorm LayerNorm_new(int64_t normalized_shape) {
return (LayerNorm){.weight = Tensor_ones(({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve13_0 = normalized_shape; rt_vec_push(&_v, &_ve13_0); _v; })), .bias = Tensor_zeros(({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve13_0 = normalized_shape; rt_vec_push(&_v, &_ve13_0); _v; })), .eps = (0LL - 5LL)};
}

Tensor LayerNorm_forward(LayerNorm* self, Tensor* input) {
return input /* clone ptr */;
}

Embedding Embedding_new(int64_t num_embeddings, int64_t embedding_dim) {
return (Embedding){.weight = Tensor_rand(({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve13_0 = num_embeddings; rt_vec_push(&_v, &_ve13_0); int64_t _ve13_1 = embedding_dim; rt_vec_push(&_v, &_ve13_1); _v; }))};
}

Tensor Embedding_forward(Embedding* self, Tensor* indices) {
int64_t idx = (int64_t)(Tensor_item(indices));
Tensor result = Tensor_zeros(({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve13_0 = (*(int64_t*)rt_vec_get(((*self).weight).shape, 1LL)); rt_vec_push(&_v, &_ve13_0); _v; }));
for (int64_t j = 0LL; j < (*(int64_t*)rt_vec_get(((*self).weight).shape, 1LL)); j++) {
(result.data[j] = (*(int64_t*)rt_vec_get(((*self).weight).data, ((idx * (*(int64_t*)rt_vec_get(((*self).weight).shape, 1LL))) + j))));
}
;
return result;
}

Dropout Dropout_new(double rate) {
return (Dropout){.rate = rate};
}

Tensor Dropout_forward(Dropout* self, Tensor* input) {
return input /* clone ptr */;
}

MultiheadAttention MultiheadAttention_new(int64_t embed_dim, int64_t num_heads) {
return (MultiheadAttention){.embed_dim = embed_dim, .num_heads = num_heads, .q_proj = Linear_new(embed_dim, embed_dim), .k_proj = Linear_new(embed_dim, embed_dim), .v_proj = Linear_new(embed_dim, embed_dim), .out_proj = Linear_new(embed_dim, embed_dim)};
}

Tensor MultiheadAttention_forward(MultiheadAttention* self, Tensor* query, Tensor* key, Tensor* value) {
Tensor q = Linear_forward(&((*self).q_proj), query);
Tensor k = Linear_forward(&((*self).k_proj), key);
Tensor v = Linear_forward(&((*self).v_proj), value);
Tensor attn = dot(({ Tensor _atmp_0 = (&(q)); &_atmp_0; }), ({ Tensor _atmp_1 = (&(k)); &_atmp_1; }));
Tensor scaled = Tensor_scalar((Tensor_item(&(attn)) / sqrt((double)((*self).embed_dim))));
Tensor weighted = mul(({ Tensor _atmp_0 = (&(scaled)); &_atmp_0; }), ({ Tensor _atmp_1 = (&(v)); &_atmp_1; }));
return Linear_forward(&((*self).out_proj), ({ Tensor _matmp_0 = (&(weighted)); &_matmp_0; }));
}

TransformerBlock TransformerBlock_new(int64_t embed_dim, int64_t num_heads, int64_t ff_dim) {
return (TransformerBlock){.self_attn = MultiheadAttention_new(embed_dim, num_heads), .fc1 = Linear_new(embed_dim, ff_dim), .fc2 = Linear_new(ff_dim, embed_dim), .norm1 = LayerNorm_new(embed_dim), .norm2 = LayerNorm_new(embed_dim)};
}

Tensor TransformerBlock_forward(TransformerBlock* self, Tensor* x) {
Tensor a = MultiheadAttention_forward(&((*self).self_attn), x, x, x);
Tensor n1 = LayerNorm_forward(&((*self).norm1), ({ Tensor _matmp_0 = (&(a)); &_matmp_0; }));
Tensor f = Linear_forward(&((*self).fc1), ({ Tensor _matmp_0 = (&(n1)); &_matmp_0; }));
Tensor activated = relu(({ Tensor _atmp_0 = (&(f)); &_atmp_0; }));
Tensor f2 = Linear_forward(&((*self).fc2), ({ Tensor _matmp_0 = (&(activated)); &_matmp_0; }));
return LayerNorm_forward(&((*self).norm2), ({ Tensor _matmp_0 = (&(f2)); &_matmp_0; }));
}

Trainer Trainer_new(int64_t epochs, int64_t batch_size, double lr) {
return (Trainer){.epochs = epochs, .batch_size = batch_size, .learning_rate = lr};
}

NyxVec Trainer_train(Trainer* self, void* model_fn, Tensor* data, Tensor* targets) {
NyxVec losses = rt_vec_new(sizeof(int64_t));
for (int64_t epoch = 0LL; epoch < (*self).epochs; epoch++) {
int64_t pred = model_fn(data);
Tensor loss = mse_loss(({ int64_t _atmp_0 = (&(pred)); &_atmp_0; }), targets);
rt_vec_push(&losses, ({ double _tmp_13 = (int64_t)(Tensor_item(&(loss))); &_tmp_13; }));
}
;
return losses;
}

DataLoader DataLoader_new(Tensor data, Tensor targets, int64_t batch_size) {
return (DataLoader){.data = data, .targets = targets, .batch_size = batch_size, .shuffle = 0};
}

DataLoader DataLoader_with_shuffle(Tensor data, Tensor targets, int64_t batch_size, int shuffle) {
return (DataLoader){.data = data, .targets = targets, .batch_size = batch_size, .shuffle = shuffle};
}

int64_t DataLoader_len(DataLoader* self) {
return ((*(int64_t*)rt_vec_get(((*self).data).shape, 0LL)) / (*self).batch_size);
}

NyxVec DataLoader_batches(DataLoader* self) {
NyxVec batches = rt_vec_new(sizeof(int64_t));
int64_t n = (*(int64_t*)rt_vec_get(((*self).data).shape, 0LL));
int64_t i = 0LL;
while (((i + (*self).batch_size) <= n)) {
NyxVec batch_data = rt_vec_new(sizeof(int64_t));
NyxVec batch_target = rt_vec_new(sizeof(int64_t));
for (int64_t j = 0LL; j < (*self).batch_size; j++) {
for (int64_t k = 0LL; k < (*(int64_t*)rt_vec_get(((*self).data).shape, 1LL)); k++) {
rt_vec_push(&batch_data, ({ int64_t _tmp_14 = (int64_t)((*(int64_t*)rt_vec_get(((*self).data).data, (((i + j) * (*(int64_t*)rt_vec_get(((*self).data).shape, 1LL))) + k)))); &_tmp_14; }));
}
;
rt_vec_push(&batch_target, ({ int64_t _tmp_15 = (int64_t)((*(int64_t*)rt_vec_get(((*self).targets).data, (i + j)))); &_tmp_15; }));
}
;
Tensor x = Tensor_new(batch_data, ({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve16_0 = (*self).batch_size; rt_vec_push(&_v, &_ve16_0); int64_t _ve16_1 = (*(int64_t*)rt_vec_get(((*self).data).shape, 1LL)); rt_vec_push(&_v, &_ve16_1); _v; }));
Tensor y = Tensor_new(batch_target, ({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve16_0 = (*self).batch_size; rt_vec_push(&_v, &_ve16_0); _v; }));
rt_vec_push(&batches, ({ NyxTuple2 _tmp_16 = (int64_t)(({ NyxTuple2 _tuple_17 = {.f0 = (void*)({ Tensor _stmp_0 = x; &_stmp_0; }), .f1 = (void*)({ Tensor _stmp_1 = y; &_stmp_1; }) }; _tuple_17; })); &_tmp_16; }));
i = (i + (*self).batch_size);
}
;
return batches;
}

Tensor one_hot(Tensor* indices, int64_t num_classes) {
Arena* __arena = arena_create();
int64_t n = (*indices).data.len;
NyxVec data = rt_vec_new(sizeof(int64_t));
for (int64_t i = 0LL; i < n; i++) {
int64_t cls = (int64_t)((*(int64_t*)rt_vec_get((*indices).data, i)));
for (int64_t j = 0LL; j < num_classes; j++) {
({ Tensor __if_res_19; if ((j == cls)) {
rt_vec_push(&data, ({ double _tmp_19 = (int64_t)(1); &_tmp_19; }));
} else {
rt_vec_push(&data, ({ double _tmp_20 = (int64_t)(0); &_tmp_20; }));
}
__if_res_19; });
}
;
}
;
arena_destroy(__arena);
return Tensor_new(data, ({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve21_0 = n; rt_vec_push(&_v, &_ve21_0); int64_t _ve21_1 = num_classes; rt_vec_push(&_v, &_ve21_1); _v; }));
}

Tensor arange(double start, double end, double step) {
Arena* __arena = arena_create();
NyxVec data = rt_vec_new(sizeof(int64_t));
double v = start;
while ((v < end)) {
rt_vec_push(&data, ({ double _tmp_21 = (int64_t)(v); &_tmp_21; }));
v = (v + step);
}
;
arena_destroy(__arena);
return Tensor_from_slice(data);
}

Tensor linspace(double start, double end, int64_t steps) {
Arena* __arena = arena_create();
NyxVec data = rt_vec_new(sizeof(int64_t));
double step = ((end - start) / (double)((steps - 1LL)));
for (int64_t i = 0LL; i < steps; i++) {
rt_vec_push(&data, ({ double _tmp_22 = (int64_t)((start + ((double)(i) * step))); &_tmp_22; }));
}
;
arena_destroy(__arena);
return Tensor_from_slice(data);
}

