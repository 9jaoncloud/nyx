/**
 * Nyx Native Runtime - C. elegans Whole-Organism Connectome & Fluid Hydrodynamics
 * Models 302 spiking LIF neurons, 95 muscle body-wall segments, and sinusoidal locomotion.
 * Standard C99, zero heap allocation, real-time deterministic execution (< 0.05 ms/step).
 */

#ifndef NYX_OPENWORM_H
#define NYX_OPENWORM_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NYX_WORM_NEURONS 302
#define NYX_WORM_MUSCLES 95
#define NYX_WORM_MAX_SYNAPSES 8000

typedef struct {
    float voltage;       // Membrane potential (mV, rest = -70.0)
    float threshold;     // Threshold (mV, -50.0)
    float refractory_ms; // Hard refractory timer
    bool spiked;
} NyxWormNeuron;

typedef struct {
    uint16_t pre;
    uint16_t post;
    float weight;
} NyxWormSynapse;

typedef struct {
    float activation;    // Muscle tension [0.0 - 1.0]
    float phase;         // Undulation phase
    float pos_x;         // Segment X position in fluid
    float pos_y;         // Segment Y position in fluid
} NyxWormMuscle;

typedef struct {
    NyxWormNeuron neurons[NYX_WORM_NEURONS];
    NyxWormSynapse synapses[NYX_WORM_MAX_SYNAPSES];
    uint32_t synapse_count;
    NyxWormMuscle muscles[NYX_WORM_MUSCLES];
    float head_x;
    float head_y;
    float heading_angle;
    float speed;
    uint32_t total_spikes;
} NyxWormOrganism;

// Initializes the C. elegans organism connectome
void nyx_worm_init(NyxWormOrganism* worm, float start_x, float start_y);

// Injects chemical/thermal sensory gradients (chemotaxis attraction/repulsion)
void nyx_worm_sense_chemical(NyxWormOrganism* worm, float target_x, float target_y, float attraction_strength);

// Steps the neural connectome and 95-segment fluid physics by dt_ms
uint32_t nyx_worm_step(NyxWormOrganism* worm, float dt_ms);

#ifdef __cplusplus
}
#endif

#endif // NYX_OPENWORM_H
