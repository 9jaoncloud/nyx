/**
 * Nyx Native Runtime - 6-Legged Hexapod Bio-Robotics & Central Pattern Generators (CPG)
 * Biomechanical tripod gait kinematics, obstacle evasion, and terrain adaptation.
 */

#ifndef NYX_NEUROMECH_H
#define NYX_NEUROMECH_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float coxa_angle;    // Horizontal hip sweep (rad)
    float femur_angle;   // Vertical thigh lift (rad)
    float tibia_angle;   // Knee reach (rad)
    float tip_x;         // Foot contact world X
    float tip_y;         // Foot contact world Y
    float tip_z;         // Foot contact height Z
    bool is_grounded;    // Stance vs Swing phase
    float phase;         // Oscillator phase [0 - 2PI]
} NyxHexapodLeg;

typedef struct {
    float pos_x;
    float pos_y;
    float heading;
    float linear_velocity;
    float angular_velocity;
    NyxHexapodLeg legs[6]; // 6 insect legs (tripod gait: L1, R2, L3 vs R1, L2, R3)
    float cpg_frequency;   // Gait frequency (Hz)
    float battery_power_mw;// Power consumption in milliwatts
    uint32_t step_count;
} NyxHexapodBot;

// Initializes hexapod bio-robot
void nyx_hexapod_init(NyxHexapodBot* bot, float start_x, float start_y);

// Commands forward velocity [-1.0 to 1.0] and turn rate [-1.0 to 1.0]
void nyx_hexapod_drive(NyxHexapodBot* bot, float throttle, float steer);

// Steps CPG non-linear coupled oscillators and forward kinematics
void nyx_hexapod_step(NyxHexapodBot* bot, float dt_sec);

#ifdef __cplusplus
}
#endif

#endif // NYX_NEUROMECH_H
