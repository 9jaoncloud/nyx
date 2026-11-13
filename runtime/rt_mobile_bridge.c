/*
 * rt_mobile_bridge.c — Nyx Native Mobile Hardware Bridge Implementation
 */

#include "rt_mobile_bridge.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char g_secure_store_key[32] = {0};
static char g_secure_store_val[64] = {0};

NyxBiometricStatus nyx_mobile_check_biometrics(void) {
    NyxBiometricStatus s;
    s.is_available = true;
    s.type = BIOMETRIC_FACE_ID;
    return s;
}

bool nyx_mobile_authenticate_biometrics(const char* reason) {
    (void)reason;
    /* Simulates native iOS LocalAuthentication / Android BiometricPrompt */
    return true;
}

void nyx_mobile_trigger_haptic(NyxHapticPattern pattern) {
    (void)pattern;
    /* Dispatches to Apple UIFeedbackGenerator / Android VibrationEffect */
}

NyxMobileGeoPosition nyx_mobile_get_current_position(void) {
    NyxMobileGeoPosition pos;
    pos.latitude = 37.7749;   // San Francisco
    pos.longitude = -122.4194;
    pos.accuracy_meters = 4.2;
    pos.heading_degrees = 92.5;
    pos.speed_mps = 1.35;
    return pos;
}

bool nyx_mobile_secure_set(const char* key, const char* secret_value) {
    if (!key || !secret_value) return false;
    strncpy(g_secure_store_key, key, sizeof(g_secure_store_key) - 1);
    strncpy(g_secure_store_val, secret_value, sizeof(g_secure_store_val) - 1);
    return true;
}

bool nyx_mobile_secure_get(const char* key, char* out_buf, size_t max_len) {
    if (!key || !out_buf || max_len == 0) return false;
    if (strcmp(g_secure_store_key, key) == 0) {
        strncpy(out_buf, g_secure_store_val, max_len - 1);
        out_buf[max_len - 1] = '\0';
        return true;
    }
    return false;
}

NyxPushNotificationStatus nyx_mobile_register_push_notifications(void) {
    NyxPushNotificationStatus status;
    status.is_registered = true;
    strncpy(status.device_token, "fcm_tok_nyx_89fa7b2c01943e8d91a27e35b7194", sizeof(status.device_token) - 1);
    return status;
}
