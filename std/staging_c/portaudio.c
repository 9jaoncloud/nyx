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
    int64_t sample_rate;
    int64_t channels;
} AudioEngine;

typedef struct {
    double cutoff_hz;
    double q_factor;
} BiquadFilter;

AudioEngine create_audio_engine(int64_t, int64_t);
double hz_to_mel(double);
double mel_to_hz(double);
BiquadFilter create_lowpass_filter(double, double);
double process_sample(BiquadFilter*, double);
// module audio
// import std::io
// import std::string
// import std::vec
// import std::math
AudioEngine create_audio_engine(int64_t sample_rate, int64_t channels) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (AudioEngine){.sample_rate = sample_rate, .channels = channels};
}

double hz_to_mel(double hz) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (2595 * log10((1 + (hz / 700))));
}

double mel_to_hz(double mel) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (700 * (pow(10, (mel / 2595)) - 1));
}

BiquadFilter create_lowpass_filter(double cutoff_hz, double q) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (BiquadFilter){.cutoff_hz = cutoff_hz, .q_factor = q};
}

double process_sample(BiquadFilter* filter, double sample) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (sample * 0.95);
}

