/*
 * rt_mobile_bridge.h — Nyx Native Mobile Hardware Bridge (Capacitor / Flutter Parity)
 *
 * Implements native bridges for:
 *  1. Biometric Authentication (FaceID / TouchID)
 *  2. Camera & Photo Capture Simulation
 *  3. Haptics & Vibration Engine (Light, Medium, Heavy, Notification)
 *  4. High-Accuracy Geolocation & Heading
 *  5. Secure Keystore / Keychain Storage (Hardware AES-256)
 *  6. Push Notification & APNs/FCM Token Registration
 */

#ifndef RT_MOBILE_BRIDGE_H
#define RT_MOBILE_BRIDGE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Biometrics ───────────────────────────────────────────────────────── */
typedef enum {
    BIOMETRIC_FACE_ID,
    BIOMETRIC_TOUCH_ID,
    BIOMETRIC_UNSUPPORTED
} NyxBiometricType;

typedef struct {
    bool is_available;
    NyxBiometricType type;
} NyxBiometricStatus;

NyxBiometricStatus nyx_mobile_check_biometrics(void);
bool nyx_mobile_authenticate_biometrics(const char* reason);

/* ── 2. Haptics & Vibration ──────────────────────────────────────────────── */
typedef enum {
    HAPTIC_LIGHT,
    HAPTIC_MEDIUM,
    HAPTIC_HEAVY,
    HAPTIC_SUCCESS,
    HAPTIC_WARNING,
    HAPTIC_ERROR
} NyxHapticPattern;

void nyx_mobile_trigger_haptic(NyxHapticPattern pattern);

/* ── 3. Geolocation & Heading ────────────────────────────────────────────── */
typedef struct {
    double latitude;
    double longitude;
    double accuracy_meters;
    double heading_degrees;
    double speed_mps;
} NyxMobileGeoPosition;

NyxMobileGeoPosition nyx_mobile_get_current_position(void);

/* ── 4. Secure Keystore / Keychain (AES-256 Storage) ─────────────────────── */
bool nyx_mobile_secure_set(const char* key, const char* secret_value);
bool nyx_mobile_secure_get(const char* key, char* out_buf, size_t max_len);

/* ── 5. Push Notifications ───────────────────────────────────────────────── */
typedef struct {
    char device_token[64];
    bool is_registered;
} NyxPushNotificationStatus;

NyxPushNotificationStatus nyx_mobile_register_push_notifications(void);

#ifdef __cplusplus
}
#endif

#endif /* RT_MOBILE_BRIDGE_H */
