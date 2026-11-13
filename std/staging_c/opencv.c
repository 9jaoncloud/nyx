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
    Image handle;
} ImageObj;

ImageObj ImageObj_new(int64_t, int64_t, int64_t, int64_t);
NyxOption ImageObj_open(rt_string_t);
void ImageObj_release(ImageObj*);
int64_t ImageObj_width(ImageObj*);
int64_t ImageObj_height(ImageObj*);
int64_t ImageObj_channels(ImageObj*);
void ImageObj_save(ImageObj*, rt_string_t);
ImageObj ImageObj_canny(ImageObj*, double, double);
ImageObj ImageObj_gaussian_blur(ImageObj*, int64_t);
void ImageObj_draw_rect(ImageObj*, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);
ImageObj ImageObj_resize(ImageObj*, int64_t, int64_t);
// module vision
// import prelude::*
extern Image cv_create_image(int64_t p0, int64_t p1, int64_t p2, int64_t p3);
extern void cv_release_image(Image p0);
extern Image cv_clone_image(Image p0);
extern Image cv_imread(rt_string_t p0, int64_t p1);
extern void cv_imwrite(rt_string_t p0, Image p1);
extern void cv_cvt_color(Image p0, Image p1, int64_t p2);
extern void cv_canny(Image p0, Image p1, double p2, double p3);
extern void cv_gaussian_blur(Image p0, Image p1, int64_t p2);
extern void cv_median_blur(Image p0, Image p1, int64_t p2);
extern void cv_erode(Image p0, Image p1, Image p2, int64_t p3);
extern void cv_dilate(Image p0, Image p1, Image p2, int64_t p3);
extern Cascade cv_create_cascade(rt_string_t p0);
extern void cv_cascade_detect_objects(Cascade p0, Image p1, int64_t p2);
extern void cv_release_cascade(Cascade p0);
extern VideoCapture cv_video_capture_open(int64_t p0);
extern void cv_video_capture_release(VideoCapture p0);
extern int64_t cv_video_capture_read(VideoCapture p0, Image p1);
extern void cv_draw_rect(Image p0, int64_t p1, int64_t p2, int64_t p3, int64_t p4, int64_t p5, int64_t p6);
extern void cv_draw_circle(Image p0, int64_t p1, int64_t p2, int64_t p3, int64_t p4, int64_t p5);
extern int64_t cv_get_width(Image p0);
extern int64_t cv_get_height(Image p0);
extern int64_t cv_get_channels(Image p0);
extern void cv_resize(Image p0, Image p1, int64_t p2, int64_t p3);
ImageObj ImageObj_new(int64_t width, int64_t height, int64_t depth, int64_t channels) {
return (ImageObj){.handle = cv_create_image(width, height, depth, channels)};
}

NyxOption ImageObj_open(rt_string_t path) {
void* h = cv_imread(path, 1LL);
if ((h == 0LL)) {
return rt_option_none();
} else {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ ImageObj _stmp_ = (ImageObj){.handle = h}; &_stmp_; }); _tmp; });
}
}

void ImageObj_release(ImageObj* self) {
cv_release_image(self.handle);
}

int64_t ImageObj_width(ImageObj* self) {
return cv_get_width((*self).handle);
}

int64_t ImageObj_height(ImageObj* self) {
return cv_get_height((*self).handle);
}

int64_t ImageObj_channels(ImageObj* self) {
return cv_get_channels((*self).handle);
}

void ImageObj_save(ImageObj* self, rt_string_t path) {
cv_imwrite(path, (*self).handle);
}

ImageObj ImageObj_canny(ImageObj* self, double low, double high) {
ImageObj dst = ImageObj_new(ImageObj_width(self), ImageObj_height(self), 8LL, 1LL);
cv_canny((*self).handle, dst.handle, low, high);
return dst;
}

ImageObj ImageObj_gaussian_blur(ImageObj* self, int64_t ksize) {
ImageObj dst = ImageObj_new(ImageObj_width(self), ImageObj_height(self), 8LL, ImageObj_channels(self));
cv_gaussian_blur((*self).handle, dst.handle, ksize);
return dst;
}

void ImageObj_draw_rect(ImageObj* self, int64_t x, int64_t y, int64_t w, int64_t h, int64_t color, int64_t thickness) {
cv_draw_rect((*self).handle, x, y, w, h, color, thickness);
}

ImageObj ImageObj_resize(ImageObj* self, int64_t width, int64_t height) {
ImageObj dst = ImageObj_new(width, height, 8LL, ImageObj_channels(self));
cv_resize((*self).handle, dst.handle, width, height);
return dst;
}

