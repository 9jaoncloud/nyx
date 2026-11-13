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
    double re;
    double im;
} OpticalField;

typedef struct {
    double theta;
    double phi;
} MachZehnderInterferometer;

typedef struct {
    OpticalField out0;
    OpticalField out1;
} MziOutput;

typedef struct {
    double v_pi;
    double bias;
} ElectroOpticActivation;

typedef struct {
    int64_t dim;
    NyxVec mzis;
} ClementsMesh;

typedef struct {
    ClementsMesh mesh;
    ElectroOpticActivation activation;
} PhotonicLinearLayer;

typedef struct {
    int64_t device_id;
    int is_hardware_present;
} QantNpuDevice;

OpticalField OpticalField_new(double, double);
OpticalField OpticalField_from_polar(double, double);
double OpticalField_intensity(OpticalField*);
OpticalField OpticalField_add(OpticalField*, OpticalField*);
OpticalField OpticalField_scale(OpticalField*, double);
MachZehnderInterferometer MachZehnderInterferometer_new(double, double);
MziOutput MachZehnderInterferometer_transfer(MachZehnderInterferometer*, OpticalField*, OpticalField*);
ElectroOpticActivation ElectroOpticActivation_new(double, double);
double ElectroOpticActivation_activate(ElectroOpticActivation*, double);
ClementsMesh ClementsMesh_new(int64_t);
NyxVec ClementsMesh_propagate(ClementsMesh*, NyxVec*);
NyxVec ClementsMesh_forward_intensities(ClementsMesh*, NyxVec*);
PhotonicLinearLayer PhotonicLinearLayer_new(int64_t);
NyxVec PhotonicLinearLayer_forward(PhotonicLinearLayer*, NyxVec*);
QantNpuDevice QantNpuDevice_connect(int64_t);
NyxVec QantNpuDevice_execute_mvm(QantNpuDevice*, ClementsMesh*, NyxVec*);
// module photonics
// import prelude::*
OpticalField OpticalField_new(double re, double im) {
return (OpticalField){.re = re, .im = im};
}

OpticalField OpticalField_from_polar(double amplitude, double phase) {
return (OpticalField){.re = (amplitude * cos(math, phase)), .im = (amplitude * sin(math, phase))};
}

double OpticalField_intensity(OpticalField* self) {
return (((*self).re * (*self).re) + ((*self).im * (*self).im));
}

OpticalField OpticalField_add(OpticalField* self, OpticalField* other) {
return (OpticalField){.re = ((*self).re + (*other).re), .im = ((*self).im + (*other).im)};
}

OpticalField OpticalField_scale(OpticalField* self, double factor) {
return (OpticalField){.re = ((*self).re * factor), .im = ((*self).im * factor)};
}

MachZehnderInterferometer MachZehnderInterferometer_new(double theta, double phi) {
return (MachZehnderInterferometer){.theta = theta, .phi = phi};
}

MziOutput MachZehnderInterferometer_transfer(MachZehnderInterferometer* self, OpticalField* in0, OpticalField* in1) {
double cos_t = cos(math, (*self).theta);
double sin_t = sin(math, (*self).theta);
double cos_p = cos(math, (*self).phi);
double sin_p = sin(math, (*self).phi);
double c0_re = ((cos_t * (*in0).re) - (sin_t * (*in1).re));
double c0_im = ((cos_t * (*in0).im) - (sin_t * (*in1).im));
double out0_re = ((c0_re * cos_p) - (c0_im * sin_p));
double out0_im = ((c0_re * sin_p) + (c0_im * cos_p));
double out1_re = ((sin_t * (*in0).re) + (cos_t * (*in1).re));
double out1_im = ((sin_t * (*in0).im) + (cos_t * (*in1).im));
return (MziOutput){.out0 = OpticalField_new(out0_re, out0_im), .out1 = OpticalField_new(out1_re, out1_im)};
}

ElectroOpticActivation ElectroOpticActivation_new(double v_pi, double bias) {
return (ElectroOpticActivation){.v_pi = v_pi, .bias = bias};
}

double ElectroOpticActivation_activate(ElectroOpticActivation* self, double voltage) {
double phase = (((3.141592653589793 * voltage) / (2 * (*self).v_pi)) + (*self).bias);
double c = cos(math, phase);
return (c * c);
}

ClementsMesh ClementsMesh_new(int64_t dim) {
int64_t count = ((dim * (dim - 1LL)) / 2LL);
int64_t mzis = Vec_with_capacity(count);
for (int64_t _ = 0LL; _ < count; _++) {
rt_vec_push(&mzis, ({ MachZehnderInterferometer _tmp_0 = (int64_t)(MachZehnderInterferometer_new((3.141592653589793 / 4), 0)); &_tmp_0; }));
}
;
return (ClementsMesh){.dim = dim, .mzis = mzis};
}

NyxVec ClementsMesh_propagate(ClementsMesh* self, NyxVec* inputs) {
int64_t n = (*self).dim;
NyxVec* state = inputs /* clone ptr */;
int64_t mzi_idx = 0LL;
for (int64_t layer = 0LL; layer < n; layer++) {
int64_t start = ({ int64_t __if_res_2; if (((layer % 2LL) == 0LL)) {
__if_res_2 = (int64_t)(0LL);
} else {
__if_res_2 = (int64_t)(1LL);
}
__if_res_2; });
int64_t i = start;
while (((i + 1LL) < n)) {
({ if ((mzi_idx < (*self).mzis.len)) {
int64_t mzi = (&((*(int64_t*)rt_vec_get((*self).mzis, mzi_idx))));
int64_t res = int64_t_transfer(mzi, (&((*(int64_t*)rt_vec_get(*state, i)))), (&((*(int64_t*)rt_vec_get(*state, (i + 1LL))))));
(state[i] = res.out0);
(state[(i + 1LL)] = res.out1);
mzi_idx = (mzi_idx + 1LL);
}
});
i = (i + 2LL);
}
;
}
;
return state;
}

NyxVec ClementsMesh_forward_intensities(ClementsMesh* self, NyxVec* real_inputs) {
int64_t optical_in = Vec_with_capacity((*self).dim);
for (int64_t i = 0LL; i < (*self).dim; i++) {
double val = ({ double __if_res_4; if ((i < real_inputs->len)) {
__if_res_4 = (*(double*)rt_vec_get(*real_inputs, i));
} else {
__if_res_4 = 0;
}
__if_res_4; });
rt_vec_push(&optical_in, ({ OpticalField _tmp_4 = (int64_t)(OpticalField_from_polar(val, 0)); &_tmp_4; }));
}
;
NyxVec optical_out = ClementsMesh_propagate(self, ({ int64_t _matmp_0 = (&(optical_in)); &_matmp_0; }));
int64_t intensities = Vec_with_capacity((*self).dim);
for (int64_t i = 0LL; i < (*self).dim; i++) {
rt_vec_push(&intensities, ({ double _tmp_5 = (int64_t)(OpticalField_intensity(&((*(OpticalField*)rt_vec_get(optical_out, i))))); &_tmp_5; }));
}
;
return intensities;
}

PhotonicLinearLayer PhotonicLinearLayer_new(int64_t dim) {
return (PhotonicLinearLayer){.mesh = ClementsMesh_new(dim), .activation = ElectroOpticActivation_new(3.2, 0)};
}

NyxVec PhotonicLinearLayer_forward(PhotonicLinearLayer* self, NyxVec* x) {
NyxVec raw_mvm = ClementsMesh_forward_intensities(&((*self).mesh), x);
int64_t activated = Vec_with_capacity(raw_mvm.len);
for (int64_t i = 0LL; i < raw_mvm.len; i++) {
rt_vec_push(&activated, ({ double _tmp_6 = (int64_t)(ElectroOpticActivation_activate(&((*self).activation), (*(double*)rt_vec_get(raw_mvm, i)))); &_tmp_6; }));
}
;
return activated;
}

QantNpuDevice QantNpuDevice_connect(int64_t device_id) {
return (QantNpuDevice){.device_id = device_id, .is_hardware_present = 0};
}

NyxVec QantNpuDevice_execute_mvm(QantNpuDevice* self, ClementsMesh* mesh, NyxVec* inputs) {
return ClementsMesh_forward_intensities(mesh, inputs);
}

