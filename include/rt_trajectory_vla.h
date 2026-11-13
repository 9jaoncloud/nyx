/*
 * rt_trajectory_vla.h — Google DeepMind RT-2 & RT-Trajectory (Vision-Language-Action)
 *
 * Implements:
 *  1. Natural Language Instruction to 6-DoF End-Effector Trajectory Waypoints
 *  2. 2D/3D Trajectory Sketch Parsing and Spline Interpolation
 *  3. Gripper Actuation & Force Feedback Constraints
 *  4. Microsecond Kinematic Execution in Zero-GC Region Bump Frames
 */

#ifndef RT_TRAJECTORY_VLA_H
#define RT_TRAJECTORY_VLA_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RT_MAX_WAYPOINTS 64

typedef struct {
    double x; // meters
    double y; // meters
    double z; // meters
    double roll_rad;
    double pitch_rad;
    double yaw_rad;
    double gripper_open_ratio; // 0.0 (fully closed) to 1.0 (fully open)
    double timestamp_sec;
} RobotPose6DoF;

typedef struct {
    char task_description[128];
    RobotPose6DoF waypoints[RT_MAX_WAYPOINTS];
    size_t waypoint_count;
    double total_trajectory_duration_sec;
    bool collision_free_verified;
    double confidence_score;
} RoboticTrajectoryPlan;

RoboticTrajectoryPlan rt_vla_plan_trajectory(const char* prompt, RobotPose6DoF initial_pose);
void rt_vla_interpolate_spline(const RoboticTrajectoryPlan* plan, double time_t, RobotPose6DoF* out_pose);

#ifdef __cplusplus
}
#endif

#endif /* RT_TRAJECTORY_VLA_H */
