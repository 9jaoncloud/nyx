#ifndef RT_ROBOTICS_H
#define RT_ROBOTICS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ── 1. 3D Vector, Quaternion & 4x4 Homogeneous Matrix ── */

typedef struct {
    double x, y, z;
} NyxVec3;

typedef struct {
    double w, x, y, z;
} NyxQuat;

typedef struct {
    double m[4][4];
} NyxMat4;

static inline NyxMat4 rt_mat4_identity(void) {
    NyxMat4 res;
    memset(&res, 0, sizeof(NyxMat4));
    res.m[0][0] = 1.0; res.m[1][1] = 1.0; res.m[2][2] = 1.0; res.m[3][3] = 1.0;
    return res;
}

static inline NyxMat4 rt_mat4_multiply(NyxMat4 a, NyxMat4 b) {
    NyxMat4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.m[i][j] = 0.0;
            for (int k = 0; k < 4; k++) {
                res.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return res;
}

/* ── 2. Denavit-Hartenberg (DH) Forward Kinematics ── */

typedef struct {
    double a;     // Link length along X_i
    double alpha; // Link twist about X_i (radians)
    double d;     // Link offset along Z_{i-1}
    double theta; // Joint angle about Z_{i-1} (radians)
} NyxDhJoint;

static inline NyxMat4 rt_dh_transform(double a, double alpha, double d, double theta) {
    NyxMat4 T;
    double ct = cos(theta), st = sin(theta);
    double ca = cos(alpha), sa = sin(alpha);

    T.m[0][0] = ct;  T.m[0][1] = -st * ca;  T.m[0][2] = st * sa;   T.m[0][3] = a * ct;
    T.m[1][0] = st;  T.m[1][1] = ct * ca;   T.m[1][2] = -ct * sa;  T.m[1][3] = a * st;
    T.m[2][0] = 0.0; T.m[2][1] = sa;        T.m[2][2] = ca;        T.m[2][3] = d;
    T.m[3][0] = 0.0; T.m[3][1] = 0.0;       T.m[3][2] = 0.0;       T.m[3][3] = 1.0;
    return T;
}

/* ── 3. Quintic Polynomial Trajectory Spline ── */

// Computes position, velocity, and acceleration at normalized time t in [0, T]
static inline void rt_trajectory_quintic(
    double t, double T,
    double q0, double v0, double a0,
    double q1, double v1, double a1,
    double* pos_out, double* vel_out, double* acc_out
) {
    if (T <= 0.0) T = 1.0;
    if (t < 0.0) t = 0.0;
    if (t > T) t = T;

    double h = q1 - q0;
    double T2 = T * T, T3 = T2 * T, T4 = T3 * T, T5 = T4 * T;

    double a_0 = q0;
    double a_1 = v0;
    double a_2 = 0.5 * a0;
    double a_3 = (20.0 * h - (8.0 * v1 + 12.0 * v0) * T - (3.0 * a0 - a1) * T2) / (2.0 * T3);
    double a_4 = (-30.0 * h + (14.0 * v1 + 16.0 * v0) * T + (3.0 * a0 - 2.0 * a1) * T2) / (2.0 * T4);
    double a_5 = (12.0 * h - 6.0 * (v1 + v0) * T + (a1 - a0) * T2) / (2.0 * T5);

    double t2 = t * t, t3 = t2 * t, t4 = t3 * t, t5 = t4 * t;

    if (pos_out) *pos_out = a_0 + a_1 * t + a_2 * t2 + a_3 * t3 + a_4 * t4 + a_5 * t5;
    if (vel_out) *vel_out = a_1 + 2.0 * a_2 * t + 3.0 * a_3 * t2 + 4.0 * a_4 * t3 + 5.0 * a_5 * t4;
    if (acc_out) *acc_out = 2.0 * a_2 + 6.0 * a_3 * t + 12.0 * a_4 * t2 + 20.0 * a_5 * t3;
}

/* ── 4. PID Controller with Anti-Windup ── */

typedef struct {
    double kp, ki, kd;
    double integral;
    double prev_error;
    double min_out, max_out;
} NyxPidController;

static inline double rt_pid_update(NyxPidController* pid, double setpoint, double measured, double dt) {
    if (dt <= 0.0) dt = 0.001;
    double error = setpoint - measured;
    pid->integral += error * dt;

    // Clamping anti-windup
    if (pid->integral > pid->max_out) pid->integral = pid->max_out;
    if (pid->integral < pid->min_out) pid->integral = pid->min_out;

    double derivative = (error - pid->prev_error) / dt;
    pid->prev_error = error;

    double output = (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
    if (output > pid->max_out) output = pid->max_out;
    if (output < pid->min_out) output = pid->min_out;
    return output;
}

#endif // RT_ROBOTICS_H
