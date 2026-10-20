#ifndef RT_ANDROID_H
#define RT_ANDROID_H

#ifdef __ANDROID__
#include <android/native_activity.h>
#include <android/input.h>
#include <android/log.h>

#define NYX_LOG_TAG "NyxApp"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, NYX_LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, NYX_LOG_TAG, __VA_ARGS__)

typedef struct {
    int x;
    int y;
    int action; // 0: down, 1: up, 2: move
    int pointer_count;
} NyxAndroidTouchEvent;

void nyx_android_init(ANativeActivity* activity);
void nyx_android_poll_events(void);
NyxAndroidTouchEvent nyx_android_get_last_touch(void);
#endif // __ANDROID__

#endif // RT_ANDROID_H
