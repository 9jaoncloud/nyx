/*
 * Nyx Connectome & Spiking Neuromorphic Subsystem
 * Bio-inspired Whole-Brain Connectome Engine (Drosophila / Fly Brain 140K Architecture)
 * Zero-GC Lexical Regions, Event-Driven Spiking Neurons, Optic Flow EMD & Mushroom Body Memory.
 */

#ifndef NYX_CONNECTOME_H
#define NYX_CONNECTOME_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Neuron Types in the Biological Fly Connectome */
typedef enum {
    NYX_NEURON_SENSORY_VISION  = 0, // Retinotopic photoreceptors & Lamina/Medulla
    NYX_NEURON_SENSORY_OLFACTORY = 1, // Antennal lobe projection neurons
    NYX_NEURON_MUSHROOM_BODY   = 2, // Kenyon cells (Associative memory)
    NYX_NEURON_CENTRAL_COMPLEX = 3, // Ring neurons (Vector navigation & compass)
    NYX_NEURON_INTERNEURON     = 4, // Local processing & lateral inhibition
    NYX_NEURON_DESCENDING_MOTOR= 5  // Ventral nerve cord & motor control
} nyx_neuron_type_t;

/* Leaky Integrate-and-Fire (LIF) Neuron State */
typedef struct {
    float voltage;          // Current membrane potential (mV)
    float resting_v;        // Resting potential (-70.0 mV)
    float threshold_v;      // Spiking threshold (-50.0 mV)
    float leak_factor;      // Membrane capacitance decay per dt (e.g. 0.92)
    float refractory_timer; // Refractory period countdown (ms)
    bool  spiked;           // Whether neuron fired in current time-step
    uint8_t type;           // nyx_neuron_type_t
} nyx_lif_neuron_t;

/* Sparse Synaptic Connection (CSR representation) */
typedef struct {
    uint32_t target_idx;    // Target post-synaptic neuron
    float    weight;        // Synaptic conductance weight (pS)
    float    delay_ms;      // Axonal conduction delay
} nyx_synapse_t;

/* Whole Connectome Graph (Up to 140,000 Neurons / 50M Synapses) */
typedef struct {
    uint32_t num_neurons;
    uint32_t num_synapses;
    nyx_lif_neuron_t* neurons;     // Flat zero-GC contiguous buffer
    uint32_t* synapse_offsets;    // CSR row index pointers
    nyx_synapse_t* synapses;      // Flat CSR synaptic weights
    uint64_t total_spikes_fired;  // Global telemetry
    float last_dt_ms;
} nyx_connectome_t;

/* Optic Flow Elementary Motion Detector (EMD: Hassenstein-Reichardt Correlator) */
typedef struct {
    uint32_t width;
    uint32_t height;
    float* prev_luminance;
    float* delayed_luminance;
    float horizontal_velocity; // Real-time perceived optical velocity X
    float vertical_velocity;   // Real-time perceived optical velocity Y
    float expansion_rate;      // Looming collision threat detection
} nyx_optic_flow_emd_t;

/* Mushroom Body Associative One-Shot Memory */
typedef struct {
    uint32_t num_projection_neurons; // Input channels (e.g. 512)
    uint32_t num_kenyon_cells;       // Sparse projection layer (e.g. 2000)
    uint32_t num_output_neurons;     // Decision channels (e.g. 32)
    float* projection_weights;       // Fixed sparse binary random matrix
    float* output_weights;           // Dopamine-modulated plastic weights
} nyx_mushroom_body_t;

/* Core API Functions */
nyx_connectome_t* nyx_connectome_create(uint32_t num_neurons, uint32_t max_synapses);
void nyx_connectome_destroy(nyx_connectome_t* brain);
void nyx_connectome_add_synapse(nyx_connectome_t* brain, uint32_t pre_idx, uint32_t post_idx, float weight);
void nyx_connectome_step(nyx_connectome_t* brain, const float* input_currents, float dt_ms);
uint32_t nyx_connectome_get_spikes(const nyx_connectome_t* brain, uint32_t* out_spiking_indices, uint32_t max_out);

/* Optic Flow & Game AI Navigation */
nyx_optic_flow_emd_t* nyx_optic_flow_create(uint32_t width, uint32_t height);
void nyx_optic_flow_destroy(nyx_optic_flow_emd_t* emd);
void nyx_optic_flow_process_frame(nyx_optic_flow_emd_t* emd, const float* current_frame, float dt_ms);

/* Mushroom Body Associative Learning */
nyx_mushroom_body_t* nyx_mushroom_body_create(uint32_t n_pn, uint32_t n_kc, uint32_t n_mbon);
void nyx_mushroom_body_destroy(nyx_mushroom_body_t* mb);
void nyx_mushroom_body_train_oneshot(nyx_mushroom_body_t* mb, const float* odor_vec, uint32_t reward_action, float lr);
uint32_t nyx_mushroom_body_predict(const nyx_mushroom_body_t* mb, const float* odor_vec);

/* Pong & 2D Game Autonomous Agent Controller using Connectome */
typedef struct {
    nyx_connectome_t* brain;
    nyx_optic_flow_emd_t* vision;
    float paddle_y;
    float paddle_velocity;
    float target_confidence;
} nyx_flybrain_game_agent_t;

nyx_flybrain_game_agent_t* nyx_flybrain_agent_create(void);
void nyx_flybrain_agent_destroy(nyx_flybrain_game_agent_t* agent);
float nyx_flybrain_agent_update(nyx_flybrain_game_agent_t* agent, float ball_x, float ball_y, float ball_vx, float ball_vy, float dt_ms);

#ifdef __cplusplus
}
#endif

#endif /* NYX_CONNECTOME_H */
