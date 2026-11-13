#include "rt_android.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __ANDROID__
static ANativeActivity* g_android_activity = NULL;
static NyxAndroidTouchEvent g_last_touch = {0};

void nyx_android_init(ANativeActivity* activity) {
    g_android_activity = activity;
    LOGI("Nyx Android Runtime Initialized (Activity: %p)", activity);
}

void nyx_android_poll_events(void) {
    // Event loop polling handled via ALooper
}

NyxAndroidTouchEvent nyx_android_get_last_touch(void) {
    return g_last_touch;
}

JNIEXPORT void JNICALL Java_com_nyx_app_NyxActivity_onNativeTouch(
    JNIEnv* env, jobject obj, jint x, jint y, jint action) {
    g_last_touch.x = x;
    g_last_touch.y = y;
    g_last_touch.action = action;
}
#endif
