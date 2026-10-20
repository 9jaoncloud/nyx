/**
 * Nyx Native Runtime - 6-Legged Hexapod Bio-Robotics & Central Pattern Generators (CPG)
 */

#include "../include/nyx_neuromech.h"
#include <string.h>

void nyx_hexapod_init(NyxHexapodBot* bot, float start_x, float start_y) {
    if (!bot) return;
    memset(bot, 0, sizeof(NyxHexapodBot));

    bot->pos_x = start_x;
    bot->pos_y = start_y;
    bot->heading = 0.0f;
    bot->linear_velocity = 0.0f;
    bot->angular_velocity = 0.0f;
    bot->cpg_frequency = 2.0f; // 2.0 Hz natural stepping frequency
    bot->battery_power_mw = 120.0f; // 120 mW nominal bio-robotic draw

    // Tripod phase offsets: Group A (0, 2, 4) in phase 0; Group B (1, 3, 5) in phase PI
    for (int i = 0; i < 6; i++) {
        bot->legs[i].phase = (i % 2 == 0) ? 0.0f : 3.14159f;
        bot->legs[i].is_grounded = (i % 2 == 0);
        bot->legs[i].coxa_angle = 0.0f;
        bot->legs[i].femur_angle = 0.3f;
        bot->legs[i].tibia_angle = -0.6f;
    }
}

void nyx_hexapod_drive(NyxHexapodBot* bot, float throttle, float steer) {
    if (!bot) return;
    bot->linear_velocity = throttle * 45.0f; // Max 45 cm/s
    bot->angular_velocity = steer * 2.2f;    // Max 2.2 rad/s
}

void nyx_hexapod_step(NyxHexapodBot* bot, float dt_sec) {
    if (!bot) return;

    // Update body heading and position
    bot->heading += bot->angular_velocity * dt_sec;
    bot->pos_x += cosf(bot->heading) * bot->linear_velocity * dt_sec;
    bot->pos_y += sinf(bot->heading) * bot->linear_velocity * dt_sec;
    bot->step_count++;

    // Calculate Kuramoto / Matsuoka coupled phase for each leg
    float speed_scale = fabsf(bot->linear_velocity) + fabsf(bot->angular_velocity) * 15.0f;
    float current_freq = (speed_scale > 0.1f) ? (bot->cpg_frequency * (speed_scale / 30.0f)) : 0.0f;

    for (int i = 0; i < 6; i++) {
        NyxHexapodLeg* leg = &bot->legs[i];
        leg->phase += current_freq * 6.28318f * dt_sec;
        while (leg->phase > 6.28318f) leg->phase -= 6.28318f;

        // Grounded (stance) during [0, PI], Lifted (swing) during [PI, 2PI]
        leg->is_grounded = (leg->phase < 3.14159f);

        // Sinusoidal CPG joint trajectories
        float swing = sinf(leg->phase);
        float forward_stroke = cosf(leg->phase);

        // Individual leg angle offsets relative to thorax
        float base_angle = (i < 3) ? (0.785f - (float)i * 0.785f) : (-0.785f + (float)(i - 3) * 0.785f);
        leg->coxa_angle = base_angle + (forward_stroke * 0.35f * (bot->linear_velocity / 45.0f));
        leg->femur_angle = leg->is_grounded ? 0.25f : (0.25f + fabsf(swing) * 0.45f);
        leg->tibia_angle = leg->is_grounded ? -0.55f : (-0.55f - fabsf(swing) * 0.25f);
    }
}
