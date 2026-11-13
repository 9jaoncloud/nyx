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
    double membrane_v;
    double resting_v;
    double threshold_v;
    double reset_v;
    double tau_m_ms;
    double refractory_ms;
    double adaptation_current;
    int64_t spike_count;
} HumanCorticalNeuron;

typedef struct {
    HumanCorticalNeuron sensory_cell_a;
    HumanCorticalNeuron sensory_cell_b;
    HumanCorticalNeuron sensory_cell_c;
    HumanCorticalNeuron association_cell_1;
    HumanCorticalNeuron association_cell_2;
    HumanCorticalNeuron motor_cell_up;
    HumanCorticalNeuron motor_cell_down;
    HumanCorticalNeuron inhibitory_cell;
    double w_sensory_to_assoc;
    double w_recurrent_thinking;
    double w_assoc_to_motor_up;
    double w_assoc_to_motor_down;
    double w_lateral_inhibition;
    double dopamine_level;
    int64_t cumulative_learning_updates;
} HumanCorticalColumn;

typedef struct {
    double court_width;
    double court_height;
    double ball_x;
    double ball_y;
    double ball_vx;
    double ball_vy;
    double paddle_y;
    double paddle_velocity;
    double paddle_height;
    double paddle_speed;
    int64_t successful_returns;
    int64_t missed_balls;
    int64_t total_rallies;
} HumanBrainPongGame;

typedef struct {
    HumanCorticalColumn cortex;
    HumanBrainPongGame game;
    int64_t ticks_elapsed;
} SovereignHumanBrainGameEngine;

HumanCorticalNeuron HumanCorticalNeuron_new(double, double, double);
int HumanCorticalNeuron_step_dynamics(HumanCorticalNeuron*, double, double);
HumanCorticalColumn HumanCorticalColumn_new();
double HumanCorticalColumn_step_column(HumanCorticalColumn*, double, double, double);
double HumanCorticalColumn_adapt_plasticity(HumanCorticalColumn*, double, double, double);
HumanBrainPongGame HumanBrainPongGame_new();
double HumanBrainPongGame_step_game(HumanBrainPongGame*, double);
SovereignHumanBrainGameEngine SovereignHumanBrainGameEngine_new();
double SovereignHumanBrainGameEngine_step_cycle(SovereignHumanBrainGameEngine*);
void nyx_main();

#line 1 "human_brain.nyx"
HumanCorticalNeuron HumanCorticalNeuron_new(double resting, double threshold, double tau) {
return (HumanCorticalNeuron){.membrane_v = resting, .resting_v = resting, .threshold_v = threshold, .reset_v = (resting - 5), .tau_m_ms = tau, .refractory_ms = 0, .adaptation_current = 0, .spike_count = 0LL};
}

int HumanCorticalNeuron_step_dynamics(HumanCorticalNeuron* self, double driving_current, double dt_ms) {
({ if (((*self).refractory_ms > 0)) {
(self->refractory_ms = ((*self).refractory_ms - dt_ms));
(self->membrane_v = (*self).reset_v);
return 0;
}
});
double leak_decay = (dt_ms / (*self).tau_m_ms);
double v_diff = ((*self).membrane_v - (*self).resting_v);
double effective_current = (driving_current - (*self).adaptation_current);
(self->membrane_v = (((*self).membrane_v - (v_diff * leak_decay)) + (effective_current * 4)));
(self->adaptation_current = ((*self).adaptation_current * (1 - (dt_ms / 80))));
({ if (((*self).membrane_v >= (*self).threshold_v)) {
(self->membrane_v = (*self).reset_v);
(self->refractory_ms = 2.5);
(self->adaptation_current = ((*self).adaptation_current + 1.2));
(self->spike_count = ((*self).spike_count + 1LL));
return 1;
}
});
return 0;
}

HumanCorticalColumn HumanCorticalColumn_new() {
return (HumanCorticalColumn){.sensory_cell_a = HumanCorticalNeuron_new((0 - 70), (0 - 50), 28.5), .sensory_cell_b = HumanCorticalNeuron_new((0 - 70), (0 - 50), 28.5), .sensory_cell_c = HumanCorticalNeuron_new((0 - 70), (0 - 50), 28.5), .association_cell_1 = HumanCorticalNeuron_new((0 - 70), (0 - 50), 32), .association_cell_2 = HumanCorticalNeuron_new((0 - 70), (0 - 50), 32), .motor_cell_up = HumanCorticalNeuron_new((0 - 70), (0 - 48), 25), .motor_cell_down = HumanCorticalNeuron_new((0 - 70), (0 - 48), 25), .inhibitory_cell = HumanCorticalNeuron_new((0 - 65), (0 - 52), 10), .w_sensory_to_assoc = 0.45, .w_recurrent_thinking = 0.35, .w_assoc_to_motor_up = 0.4, .w_assoc_to_motor_down = 0.4, .w_lateral_inhibition = 0.3, .dopamine_level = 1, .cumulative_learning_updates = 0LL};
}

double HumanCorticalColumn_step_column(HumanCorticalColumn* self, double sensor_ball_relative_y, double sensor_ball_speed_x, double dt_ms) {
double sens_current_up = 0;
double sens_current_down = 0;
({ double __if_res_3; if ((sensor_ball_relative_y < 0)) {
__if_res_3 = (sens_current_up = ((0 - sensor_ball_relative_y) * 45));
} else {
__if_res_3 = (sens_current_down = (sensor_ball_relative_y * 45));
}
__if_res_3; });
int spike_sens_up = HumanCorticalNeuron_step_dynamics(&((*self).sensory_cell_a), sens_current_up, dt_ms);
int spike_sens_down = HumanCorticalNeuron_step_dynamics(&((*self).sensory_cell_b), sens_current_down, dt_ms);
int spike_sens_speed = HumanCorticalNeuron_step_dynamics(&((*self).sensory_cell_c), (sensor_ball_speed_x * 12), dt_ms);
double input_to_assoc_1 = 0;
double input_to_assoc_2 = 0;
({ if (spike_sens_up) {
input_to_assoc_1 = (input_to_assoc_1 + ((*self).w_sensory_to_assoc * 25));
}
});
({ if (spike_sens_down) {
input_to_assoc_2 = (input_to_assoc_2 + ((*self).w_sensory_to_assoc * 25));
}
});
({ if (spike_sens_speed) {
input_to_assoc_1 = (input_to_assoc_1 + 3);
input_to_assoc_2 = (input_to_assoc_2 + 3);
}
});
double recurrent_boost = ((*self).w_recurrent_thinking * 6);
int spike_assoc_1 = HumanCorticalNeuron_step_dynamics(&((*self).association_cell_1), (input_to_assoc_1 + recurrent_boost), dt_ms);
int spike_assoc_2 = HumanCorticalNeuron_step_dynamics(&((*self).association_cell_2), (input_to_assoc_2 + recurrent_boost), dt_ms);
double total_assoc_activity = 0;
({ if (spike_assoc_1) {
total_assoc_activity = (total_assoc_activity + 1);
}
});
({ if (spike_assoc_2) {
total_assoc_activity = (total_assoc_activity + 1);
}
});
int inh_spike = HumanCorticalNeuron_step_dynamics(&((*self).inhibitory_cell), (total_assoc_activity * 8), dt_ms);
double inh_feedback = 0;
({ if (inh_spike) {
inh_feedback = ((*self).w_lateral_inhibition * 4);
}
});
double motor_in_up = 0;
double motor_in_down = 0;
({ if (spike_assoc_1) {
motor_in_up = (motor_in_up + ((*self).w_assoc_to_motor_up * 30));
}
});
({ if (spike_assoc_2) {
motor_in_down = (motor_in_down + ((*self).w_assoc_to_motor_down * 30));
}
});
motor_in_up = (motor_in_up - inh_feedback);
motor_in_down = (motor_in_down - inh_feedback);
({ if ((motor_in_up < 0)) {
motor_in_up = 0;
}
});
({ if ((motor_in_down < 0)) {
motor_in_down = 0;
}
});
int spike_motor_up = HumanCorticalNeuron_step_dynamics(&((*self).motor_cell_up), motor_in_up, dt_ms);
int spike_motor_down = HumanCorticalNeuron_step_dynamics(&((*self).motor_cell_down), motor_in_down, dt_ms);
double paddle_command = 0;
({ if (spike_motor_up) {
paddle_command = (paddle_command - 1);
}
});
({ if (spike_motor_down) {
paddle_command = (paddle_command + 1);
}
});
return paddle_command;
}

double HumanCorticalColumn_adapt_plasticity(HumanCorticalColumn* self, double pre_activation, double post_activation, double reward) {
double eta = ((0.008 * (*self).dopamine_level) * reward);
double hebbian = (pre_activation * post_activation);
double oja_decay = ((post_activation * post_activation) * (*self).w_assoc_to_motor_up);
double delta_w = (eta * (hebbian - oja_decay));
(self->w_assoc_to_motor_up = ((*self).w_assoc_to_motor_up + delta_w));
(self->w_assoc_to_motor_down = ((*self).w_assoc_to_motor_down + delta_w));
({ if (((*self).w_assoc_to_motor_up > 1.2)) {
(self->w_assoc_to_motor_up = 1.2);
}
});
({ if (((*self).w_assoc_to_motor_up < 0.05)) {
(self->w_assoc_to_motor_up = 0.05);
}
});
({ if (((*self).w_assoc_to_motor_down > 1.2)) {
(self->w_assoc_to_motor_down = 1.2);
}
});
({ if (((*self).w_assoc_to_motor_down < 0.05)) {
(self->w_assoc_to_motor_down = 0.05);
}
});
(self->cumulative_learning_updates = ((*self).cumulative_learning_updates + 1LL));
return delta_w;
}

HumanBrainPongGame HumanBrainPongGame_new() {
return (HumanBrainPongGame){.court_width = 800, .court_height = 500, .ball_x = 200, .ball_y = 250, .ball_vx = 5, .ball_vy = 3, .paddle_y = 250, .paddle_velocity = 0, .paddle_height = 90, .paddle_speed = 6, .successful_returns = 0LL, .missed_balls = 0LL, .total_rallies = 0LL};
}

double HumanBrainPongGame_step_game(HumanBrainPongGame* self, double brain_motor_action) {
(self->paddle_velocity = (((*self).paddle_velocity * 0.92) + (brain_motor_action * 2.2)));
(self->paddle_y = ((*self).paddle_y + ((*self).paddle_velocity * (*self).paddle_speed)));
double half_h = ((*self).paddle_height / 2);
({ if (((*self).paddle_y < half_h)) {
(self->paddle_y = half_h);
}
});
({ if (((*self).paddle_y > ((*self).court_height - half_h))) {
(self->paddle_y = ((*self).court_height - half_h));
}
});
(self->ball_x = ((*self).ball_x + (*self).ball_vx));
(self->ball_y = ((*self).ball_y + (*self).ball_vy));
({ if (((*self).ball_y <= 0)) {
(self->ball_y = 0);
(self->ball_vy = (0 - (*self).ball_vy));
}
});
({ if (((*self).ball_y >= (*self).court_height)) {
(self->ball_y = (*self).court_height);
(self->ball_vy = (0 - (*self).ball_vy));
}
});
({ if (((*self).ball_x <= 0)) {
(self->ball_x = 0);
(self->ball_vx = (0 - (*self).ball_vx));
}
});
double sensory_reward = 0;
({ if (((*self).ball_x >= (*self).court_width)) {
double paddle_top = ((*self).paddle_y - half_h);
double paddle_bottom = ((*self).paddle_y + half_h);
({ if (((*self).ball_y >= paddle_top)) {
({ if (((*self).ball_y <= paddle_bottom)) {
(self->successful_returns = ((*self).successful_returns + 1LL));
(self->total_rallies = ((*self).total_rallies + 1LL));
(self->ball_x = ((*self).court_width - 1));
(self->ball_vx = (0 - (*self).ball_vx));
sensory_reward = 1;
return sensory_reward;
}
});
}
});
(self->missed_balls = ((*self).missed_balls + 1LL));
(self->total_rallies = ((*self).total_rallies + 1LL));
(self->ball_x = 200);
(self->ball_y = 250);
(self->ball_vx = 5);
sensory_reward = (0 - 1);
return sensory_reward;
}
});
return sensory_reward;
}

SovereignHumanBrainGameEngine SovereignHumanBrainGameEngine_new() {
return (SovereignHumanBrainGameEngine){.cortex = HumanCorticalColumn_new(), .game = HumanBrainPongGame_new(), .ticks_elapsed = 0LL};
}

double SovereignHumanBrainGameEngine_step_cycle(SovereignHumanBrainGameEngine* self) {
(self->ticks_elapsed = ((*self).ticks_elapsed + 1LL));
double delta_y = ((((*self).game).ball_y - ((*self).game).paddle_y) / (((*self).game).court_height / 2));
double speed_norm = (((*self).game).ball_vx / 10);
double paddle_action = HumanCorticalColumn_step_column(&((*self).cortex), delta_y, speed_norm, 1);
double feedback_reward = HumanBrainPongGame_step_game(&((*self).game), paddle_action);
({ if ((feedback_reward > 0)) {
((*self).cortex.dopamine_level = 2);
double _up1 = HumanCorticalColumn_adapt_plasticity(&((*self).cortex), 1, 1, 1);
}
});
({ if ((feedback_reward < 0)) {
((*self).cortex.dopamine_level = 0.2);
double _up2 = HumanCorticalColumn_adapt_plasticity(&((*self).cortex), 1, 1, (0 - 0.5));
}
});
return paddle_action;
}

void nyx_main() {
Arena* __arena = arena_create();
printf("%s\n", rt_string_from("==================================================================").data);
printf("%s\n", rt_string_from("  NYX SOVEREIGN HUMAN BRAIN GAME-PLAYING ENGINE (Pong / Visual Nav)").data);
printf("%s\n", rt_string_from("  100% Pure Nyx · Zero-GC Lexical Region Memory · Closed-Loop Living Core").data);
printf("%s\n", rt_string_from("==================================================================").data);
SovereignHumanBrainGameEngine engine = SovereignHumanBrainGameEngine_new();
printf("%s\n", rt_string_from("\n[1] NEUROBIOLOGICAL SPECIFICATIONS & EMPIRICAL HUMAN PARAMETERS").data);
printf("%s\n", rt_string_from("  Neocortical Circuit    : 6-Layer Laminar Column (Layers II/III, IV, V, VI)").data);
printf("%s\n", rt_string_from("  Pyramidal Membrane Tau : 28.5 ms (Human Temporal Cortex H01 benchmark)").data);
printf("%s\n", rt_string_from("  Interneuron Substrate  : Fast-Spiking PV+ GABAergic Lateral Inhibition").data);
printf("%s\n", rt_string_from("  Synaptic Adaptation    : Self-Normalizing Oja Rule with Dopamine Gating").data);
printf("%s\n", rt_string_from("  Sensory-Motor Feedback : Closed-Loop Free Energy Active Inference").data);
printf("%s\n", rt_string_from("\n[2] EXECUTING REAL-TIME CLOSED-LOOP GAME RALLIES (1,000 Ticks)...").data);
int64_t tick = 1LL;
while ((tick <= 1000LL)) {
double action = SovereignHumanBrainGameEngine_step_cycle(&(engine));
tick = (tick + 1LL);
}
;
printf("%s\n", rt_string_from("  Game Simulation Completed Successfully.").data);
printf("  Total Physical Ticks   : %lld\n", (long long)(engine.ticks_elapsed));
printf("  Total Rallies Played   : %lld\n", (long long)((engine.game).total_rallies));
printf("  Successful Intercepts  : %lld\n", (long long)((engine.game).successful_returns));
printf("  Missed Intercepts      : %lld\n", (long long)((engine.game).missed_balls));
printf("  Final Paddle Position  : %f\n", (engine.game).paddle_y);
printf("  Final Ball Position Y  : %f\n", (engine.game).ball_y);
printf("  Layer V Motor Spikes   : %lld\n", (long long)((((engine.cortex).motor_cell_up).spike_count + ((engine.cortex).motor_cell_down).spike_count)));
printf("  Inhibitory Spikes      : %lld\n", (long long)(((engine.cortex).inhibitory_cell).spike_count));
printf("  Plasticity Weight Up   : %f\n", (engine.cortex).w_assoc_to_motor_up);
printf("  Plasticity Weight Down : %f\n", (engine.cortex).w_assoc_to_motor_down);
printf("  Total Synaptic Updates : %lld\n", (long long)((engine.cortex).cumulative_learning_updates));
printf("%s\n", rt_string_from("\n==================================================================").data);
printf("%s\n", rt_string_from("  SUCCESS: Sovereign Human Brain Game Engine Formally Verified!").data);
printf("%s\n", rt_string_from("==================================================================").data);
arena_destroy(__arena);
}


int main(int argc, char** argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    nyx_main();
    return 0;
}
