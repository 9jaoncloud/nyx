#include <string.h>
#include <stdint.h>
#include "runtime/rt_http.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_db.h"
#include "runtime/rt_io.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include <stdio.h>
#include <math.h>
#include "runtime/rt_string.h"
#include <stdlib.h>
#include "runtime/rt_option.h"
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
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

#define TAG_FocusMode_ContinuousAutoFocus 0
#define TAG_FocusMode_Macro 1
#define TAG_FocusMode_Infinity 2
#define TAG_FocusMode_ManualLock 3
typedef struct {
    int tag;
    union {
    } data;
} FocusMode;

#define TAG_BarcodeFormat_QrCode 0
#define TAG_BarcodeFormat_Ean13 1
#define TAG_BarcodeFormat_UpcA 2
#define TAG_BarcodeFormat_Code128 3
#define TAG_BarcodeFormat_DataMatrix 4
#define TAG_BarcodeFormat_Aztec 5
typedef struct {
    int tag;
    union {
    } data;
} BarcodeFormat;

#define TAG_CameraFacing_Back 0
#define TAG_CameraFacing_Front 1
#define TAG_CameraFacing_ExternalUsb 2
typedef struct {
    int tag;
    union {
    } data;
} CameraFacing;

typedef struct {
    int64_t width_px;
    int64_t height_px;
    int64_t fps_target;
} CameraResolution;

typedef struct {
    CameraFacing facing;
    CameraResolution resolution;
    FocusMode focus_mode;
    int is_torch_active;
    double exposure_compensation_ev;
    int hardware_ois_enabled;
} CameraSensorConfig;

typedef struct {
    double x;
    double y;
} Point2D;

typedef struct {
    rt_string_t raw_text;
    BarcodeFormat format;
    NyxVec corner_points;
    double decode_duration_ms;
    double confidence_score;
} BarcodeScanResult;

typedef struct {
    CameraSensorConfig config;
    int is_streaming;
    int64_t frame_counter;
} CameraController;

CameraController CameraController_new(CameraFacing);
int CameraController_start_preview_stream(void*);
void CameraController_set_torch(void*, int);
void CameraController_set_focus_point(void*, double, double);
NyxOption CameraController_scan_frame_for_barcode(void*, NyxVec);
void CameraController_stop_stream(void*);
// module camera
// import std::string
// import std::vec
// import std::option
// import std::result
CameraController CameraController_new(CameraFacing facing) {
return (CameraController){.config = (CameraSensorConfig){.facing = facing, .resolution = (CameraResolution){.width_px = 1920LL, .height_px = 1080LL, .fps_target = 60LL}, .focus_mode = (FocusMode){ .tag = TAG_FocusMode_ContinuousAutoFocus }, .is_torch_active = 0, .exposure_compensation_ev = 0, .hardware_ois_enabled = 1}, .is_streaming = 0, .frame_counter = 0LL};
}

int CameraController_start_preview_stream(void* self) {
(self->is_streaming = 1);
return 1;
}

void CameraController_set_torch(void* self, int enabled) {
((*self).config.is_torch_active = enabled);
}

void CameraController_set_focus_point(void* self, double norm_x, double norm_y) {
}

NyxOption CameraController_scan_frame_for_barcode(void* self, NyxVec allowed_formats) {
(self->frame_counter = ((*self).frame_counter + 1LL));
NyxVec corners = rt_vec_new(sizeof(int64_t));
rt_vec_push(&corners, ({ Point2D _tmp_0 = (Point2D){.x = 120, .y = 140}; &_tmp_0; }));
rt_vec_push(&corners, ({ Point2D _tmp_1 = (Point2D){.x = 480, .y = 140}; &_tmp_1; }));
rt_vec_push(&corners, ({ Point2D _tmp_2 = (Point2D){.x = 480, .y = 500}; &_tmp_2; }));
rt_vec_push(&corners, ({ Point2D _tmp_3 = (Point2D){.x = 120, .y = 500}; &_tmp_3; }));
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ BarcodeScanResult _stmp_ = (BarcodeScanResult){.raw_text = rt_string_from("https://nyx.9jaoncloud.com.ng/pay/tx_9982481"), .format = (BarcodeFormat){ .tag = TAG_BarcodeFormat_QrCode }, .corner_points = corners, .decode_duration_ms = 6.4, .confidence_score = 0.994}; &_stmp_; }); _tmp; });
}

void CameraController_stop_stream(void* self) {
(self->is_streaming = 0);
}

