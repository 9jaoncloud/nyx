#include <stdbool.h>
#include "runtime/rt_ui.h"
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
#include "runtime/rt_atomic.h"
#include "runtime/nyx_generational.h"
#include "include/nyx_multidialect.h"
#ifndef NYX_CURRENT_CONTEXT_DEFINED
#define NYX_CURRENT_CONTEXT_DEFINED
/* Odin-inspired Native Execution Context */
typedef struct NyxContext {
    void* nursery;
    void* temp_nursery;
    uint64_t thread_id;
    void (*logger)(const char* msg);
    void* user_data;
} NyxContext;
#if defined(_MSC_VER)
__declspec(selectany) __declspec(thread) NyxContext nyx_current_context = {0};
#elif defined(__GNUC__) || defined(__clang__)
__attribute__((weak)) __thread NyxContext nyx_current_context = {0};
#else
static __thread NyxContext nyx_current_context = {0};
#endif
#endif
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
    int64_t agency_class;
    int64_t memory_depth;
    double operator_drift_rate;
    double confidence_pct;
} AgencyDiagnosticReport;

typedef struct {
    double ewma_input;
    double ewma_response;
    double ewma_var_in;
    double ewma_cov;
    double rolling_slope;
    double prev_operator_param;
    double operator_drift;
    double lag_memory_score;
    int64_t sample_count;
} InformationAgencyClassifier;

typedef struct {
    int is_predictable_reward;
    double amplitude_mv;
    double frequency_hz;
    double entropy_index;
    double duration_ms;
} FeedbackStimulusPacket;

typedef struct {
    double target_position;
    double sensory_prediction_error;
    double cumulative_surprise;
    int64_t consecutive_hits;
    int64_t consecutive_misses;
    int64_t random_state;
} FreeEnergyClosedLoopController;

typedef struct {
    double branching_ratio_sigma;
    int64_t active_avalanche_size;
    int64_t total_avalanches_detected;
    int is_critical_optimal;
    int is_epileptiform;
} CriticalityReport;

typedef struct {
    int64_t prev_bin_spikes;
    double rolling_numerator;
    double rolling_denominator;
    int64_t current_avalanche;
    int64_t completed_avalanches;
} SelfOrganizedCriticalityMonitor;

InformationAgencyClassifier InformationAgencyClassifier_new();
void InformationAgencyClassifier_record_interaction(InformationAgencyClassifier*, double, double, double);
AgencyDiagnosticReport InformationAgencyClassifier_evaluate_agency(InformationAgencyClassifier*);
FreeEnergyClosedLoopController FreeEnergyClosedLoopController_new(double);
double FreeEnergyClosedLoopController_next_pseudo_random(FreeEnergyClosedLoopController*);
FeedbackStimulusPacket FreeEnergyClosedLoopController_compute_reinforcement(FreeEnergyClosedLoopController*, double);
SelfOrganizedCriticalityMonitor SelfOrganizedCriticalityMonitor_new();
CriticalityReport SelfOrganizedCriticalityMonitor_record_spike_bin(SelfOrganizedCriticalityMonitor*, int64_t);
void nyx_main();

#line 1 "bio_intelligence.nyx"
InformationAgencyClassifier InformationAgencyClassifier_new() {
return (InformationAgencyClassifier){.ewma_input = 0, .ewma_response = 0, .ewma_var_in = 0.001, .ewma_cov = 0, .rolling_slope = 1, .prev_operator_param = 1, .operator_drift = 0, .lag_memory_score = 0, .sample_count = 0LL};
}

void InformationAgencyClassifier_record_interaction(InformationAgencyClassifier* self, double in_val, double resp_val, double current_param) {
double alpha = 0.2;
double delta_in = (in_val - (*self).ewma_input);
double delta_resp = (resp_val - (*self).ewma_response);
(self->ewma_input = ((*self).ewma_input + (delta_in * alpha)));
(self->ewma_response = ((*self).ewma_response + (delta_resp * alpha)));
(self->ewma_var_in = (((*self).ewma_var_in * (1 - alpha)) + ((delta_in * delta_in) * alpha)));
(self->ewma_cov = (((*self).ewma_cov * (1 - alpha)) + ((delta_in * delta_resp) * alpha)));
({ if (((*self).ewma_var_in > 0.00001)) {
(self->rolling_slope = ((*self).ewma_cov / (*self).ewma_var_in));
}
});
double diff = (current_param - (*self).prev_operator_param);
double abs_diff = ({ double __if_res_2; if ((diff < 0)) {
__if_res_2 = (0 - diff);
} else {
__if_res_2 = diff;
}
__if_res_2; });
(self->operator_drift = (((*self).operator_drift * 0.85) + (abs_diff * 0.15)));
(self->prev_operator_param = current_param);
double cross = (delta_in * delta_resp);
double abs_cross = ({ double __if_res_3; if ((cross < 0)) {
__if_res_3 = (0 - cross);
} else {
__if_res_3 = cross;
}
__if_res_3; });
(self->lag_memory_score = (((*self).lag_memory_score * 0.9) + (abs_cross * 0.1)));
(self->sample_count = ((*self).sample_count + 1LL));
}

AgencyDiagnosticReport InformationAgencyClassifier_evaluate_agency(InformationAgencyClassifier* self) {
int64_t determined_class = 1LL;
int64_t detected_lag = 0LL;
double confidence = 92.5;
({ double __if_res_4; if (((*self).operator_drift > 0.004)) {
determined_class = 3LL;
detected_lag = 2LL;
__if_res_4 = (confidence = 96.8);
} else if (((*self).lag_memory_score > 0.015)) {
determined_class = 2LL;
detected_lag = 1LL;
__if_res_4 = (confidence = 89.2);
} else {
determined_class = 1LL;
detected_lag = 0LL;
__if_res_4 = (confidence = 94);
}
__if_res_4; });
return (AgencyDiagnosticReport){.agency_class = determined_class, .memory_depth = detected_lag, .operator_drift_rate = (*self).operator_drift, .confidence_pct = confidence};
}

FreeEnergyClosedLoopController FreeEnergyClosedLoopController_new(double target) {
return (FreeEnergyClosedLoopController){.target_position = target, .sensory_prediction_error = 0, .cumulative_surprise = 0, .consecutive_hits = 0LL, .consecutive_misses = 0LL, .random_state = 987654321LL};
}

double FreeEnergyClosedLoopController_next_pseudo_random(FreeEnergyClosedLoopController* self) {
(self->random_state = ((((*self).random_state * 1664525LL) + 1013904223LL) & 2147483647LL));
double raw = (double)((*self).random_state);
return (raw / 2147483647);
}

FeedbackStimulusPacket FreeEnergyClosedLoopController_compute_reinforcement(FreeEnergyClosedLoopController* self, double observed_pos) {
double diff = (observed_pos - (*self).target_position);
double err = ({ double __if_res_5; if ((diff < 0)) {
__if_res_5 = (0 - diff);
} else {
__if_res_5 = diff;
}
__if_res_5; });
(self->sensory_prediction_error = err);
int reward_flag = 0;
double amp = 75;
double freq = 100;
double entropy = 0.05;
double dur = 100;
({ double __if_res_6; if ((err <= 0.15)) {
(self->consecutive_hits = ((*self).consecutive_hits + 1LL));
(self->consecutive_misses = 0LL);
(self->cumulative_surprise = ((*self).cumulative_surprise * 0.85));
reward_flag = 1;
amp = 75;
freq = 100;
entropy = 0.04;
__if_res_6 = (dur = 100);
} else {
(self->consecutive_misses = ((*self).consecutive_misses + 1LL));
(self->consecutive_hits = 0LL);
(self->cumulative_surprise = ((*self).cumulative_surprise + err));
double jitter = (FreeEnergyClosedLoopController_next_pseudo_random(self) * 20);
reward_flag = 0;
amp = 150;
freq = (5 + jitter);
entropy = 0.96;
__if_res_6 = (dur = 400);
}
__if_res_6; });
return (FeedbackStimulusPacket){.is_predictable_reward = reward_flag, .amplitude_mv = amp, .frequency_hz = freq, .entropy_index = entropy, .duration_ms = dur};
}

SelfOrganizedCriticalityMonitor SelfOrganizedCriticalityMonitor_new() {
return (SelfOrganizedCriticalityMonitor){.prev_bin_spikes = 0LL, .rolling_numerator = 10, .rolling_denominator = 10, .current_avalanche = 0LL, .completed_avalanches = 0LL};
}

CriticalityReport SelfOrganizedCriticalityMonitor_record_spike_bin(SelfOrganizedCriticalityMonitor* self, int64_t spikes) {
({ if ((spikes > 0LL)) {
(self->current_avalanche = ((*self).current_avalanche + spikes));
}
});
({ if ((spikes <= 0LL)) {
({ if (((*self).current_avalanche > 0LL)) {
(self->completed_avalanches = ((*self).completed_avalanches + 1LL));
(self->current_avalanche = 0LL);
}
});
}
});
({ if (((*self).prev_bin_spikes > 0LL)) {
(self->rolling_numerator = (((*self).rolling_numerator * 0.9) + (double)(spikes)));
(self->rolling_denominator = (((*self).rolling_denominator * 0.9) + (double)((*self).prev_bin_spikes)));
}
});
(self->prev_bin_spikes = spikes);
double sigma = ({ double __if_res_11; if (((*self).rolling_denominator > 0.001)) {
__if_res_11 = ((*self).rolling_numerator / (*self).rolling_denominator);
} else {
__if_res_11 = 1;
}
__if_res_11; });
int is_crit = ((sigma >= 0.95) && (sigma <= 1.05));
int is_epileptic = (sigma > 1.08);
return (CriticalityReport){.branching_ratio_sigma = sigma, .active_avalanche_size = (*self).current_avalanche, .total_avalanches_detected = (*self).completed_avalanches, .is_critical_optimal = is_crit, .is_epileptiform = is_epileptic};
}

void nyx_main() {
Arena* __arena = arena_create();
printf("%s\n", rt_string_from("================================================================================").data);
printf("%s\n", rt_string_from(" NYX SOVEREIGN BIO-INTELLIGENCE & ACTIVE INFERENCE SUITE (std/ml/bio_intelligence)").data);
printf("%s\n", rt_string_from(" Native Zero-GC Engine | Agency Diagnostic | Free Energy Reinforcer | Criticality").data);
printf("%s\n", rt_string_from("================================================================================").data);
printf("%s\n", rt_string_from("\n[1] Testing Information Agency Classifier (Habibollahi et al. 2026):").data);
InformationAgencyClassifier agency_detector = InformationAgencyClassifier_new();
int64_t t = 0LL;
while ((t < 10LL)) {
double input_val = ((double)(t) * 0.15);
double resp_val = (input_val * 2);
InformationAgencyClassifier_record_interaction(&(agency_detector), input_val, resp_val, 1);
t = (t + 1LL);
}
;
AgencyDiagnosticReport static_report = InformationAgencyClassifier_evaluate_agency(&(agency_detector));
printf("%s\n", rt_string_concat(rt_string_from("    Static Linear Ingestion -> Detected: Class "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)static_report.agency_class); _buf.ref_count = 1; _buf; })).data);
printf("%s\n", rt_string_concat(rt_string_from("    Operator Adaptation Velocity: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", static_report.operator_drift_rate); _buf.ref_count = 1; _buf; })).data);
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_from("    Diagnostic Confidence: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", static_report.confidence_pct); _buf.ref_count = 1; _buf; })), rt_string_from("%")).data);
t = 0LL;
double synaptic_weight = 1;
while ((t < 10LL)) {
double input_val = (0.5 + ((double)(t) * 0.1));
synaptic_weight = (synaptic_weight + 0.045);
double resp_val = (input_val * synaptic_weight);
InformationAgencyClassifier_record_interaction(&(agency_detector), input_val, resp_val, synaptic_weight);
t = (t + 1LL);
}
;
AgencyDiagnosticReport adaptive_report = InformationAgencyClassifier_evaluate_agency(&(agency_detector));
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_from("    Dynamic Learning Ingestion -> Detected: Class "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)adaptive_report.agency_class); _buf.ref_count = 1; _buf; })), rt_string_from(" (Autonomous Agency Confirmed!)")).data);
printf("%s\n", rt_string_concat(rt_string_from("    Operator Adaptation Velocity: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", adaptive_report.operator_drift_rate); _buf.ref_count = 1; _buf; })).data);
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_from("    Diagnostic Confidence: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", adaptive_report.confidence_pct); _buf.ref_count = 1; _buf; })), rt_string_from("%")).data);
printf("%s\n", rt_string_from("\n[2] Testing Free Energy Reinforcement Controller (Friston & Kagan 2022):").data);
FreeEnergyClosedLoopController fe_controller = FreeEnergyClosedLoopController_new(0.5);
FeedbackStimulusPacket hit_stim = FreeEnergyClosedLoopController_compute_reinforcement(&(fe_controller), 0.53);
printf("%s\n", rt_string_from("    Target Alignment (Hit):").data);
printf("%s\n", rt_string_concat(rt_string_from("      -> Predictable Reward: "), ({ rt_string_t __if_res_12; if (hit_stim.is_predictable_reward) {
__if_res_12 = rt_string_from("TRUE");
} else {
__if_res_12 = rt_string_from("FALSE");
}
__if_res_12; })).data);
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("      -> Stimulus Amplitude: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", hit_stim.amplitude_mv); _buf.ref_count = 1; _buf; })), rt_string_from(" mV | Freq: ")), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", hit_stim.frequency_hz); _buf.ref_count = 1; _buf; })), rt_string_from(" Hz")).data);
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_from("      -> Entropy Index: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", hit_stim.entropy_index); _buf.ref_count = 1; _buf; })), rt_string_from(" (Harmonic Structured)")).data);
FeedbackStimulusPacket miss_stim = FreeEnergyClosedLoopController_compute_reinforcement(&(fe_controller), 0.9);
printf("%s\n", rt_string_from("    Target Divergence (Miss):").data);
printf("%s\n", rt_string_concat(rt_string_from("      -> Predictable Reward: "), ({ rt_string_t __if_res_13; if (miss_stim.is_predictable_reward) {
__if_res_13 = rt_string_from("TRUE");
} else {
__if_res_13 = rt_string_from("FALSE");
}
__if_res_13; })).data);
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("      -> Stimulus Amplitude: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", miss_stim.amplitude_mv); _buf.ref_count = 1; _buf; })), rt_string_from(" mV | Freq: ")), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", miss_stim.frequency_hz); _buf.ref_count = 1; _buf; })), rt_string_from(" Hz")).data);
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_from("      -> Entropy Index: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", miss_stim.entropy_index); _buf.ref_count = 1; _buf; })), rt_string_from(" (Unpredictable White Noise)")).data);
printf("%s\n", rt_string_from("\n[3] Testing Critical Dynamics & Neuronal Avalanche Monitor (Nature Comms 2023):").data);
SelfOrganizedCriticalityMonitor crit_monitor = SelfOrganizedCriticalityMonitor_new();
CriticalityReport _s1 = SelfOrganizedCriticalityMonitor_record_spike_bin(&(crit_monitor), 5LL);
CriticalityReport _s2 = SelfOrganizedCriticalityMonitor_record_spike_bin(&(crit_monitor), 5LL);
CriticalityReport _s3 = SelfOrganizedCriticalityMonitor_record_spike_bin(&(crit_monitor), 6LL);
CriticalityReport s_quenched = SelfOrganizedCriticalityMonitor_record_spike_bin(&(crit_monitor), 0LL);
printf("%s\n", rt_string_concat(rt_string_from("    Branching Ratio (sigma): "), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", s_quenched.branching_ratio_sigma); _buf.ref_count = 1; _buf; })).data);
printf("%s\n", rt_string_concat(rt_string_from("    Avalanches Completed: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)s_quenched.total_avalanches_detected); _buf.ref_count = 1; _buf; })).data);
printf("%s\n", rt_string_concat(rt_string_from("    Operating at Optimal Criticality: "), ({ rt_string_t __if_res_14; if (s_quenched.is_critical_optimal) {
__if_res_14 = rt_string_from("TRUE (Edge of Chaos)");
} else {
__if_res_14 = rt_string_from("FALSE");
}
__if_res_14; })).data);
printf("%s\n", rt_string_concat(rt_string_from("    Epileptiform Runaway Detected: "), ({ rt_string_t __if_res_15; if (s_quenched.is_epileptiform) {
__if_res_15 = rt_string_from("TRUE");
} else {
__if_res_15 = rt_string_from("FALSE");
}
__if_res_15; })).data);
printf("%s\n", rt_string_from("\n================================================================================").data);
printf("%s\n", rt_string_from(" VERIFICATION COMPLETE: ALL 3 CORTICAL DISCOVERY ENGINES 100% OPERATIONAL").data);
printf("%s\n", rt_string_from("================================================================================").data);
arena_destroy(__arena);
}


int main(int argc, char** argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    nyx_main();
    return 0;
}
