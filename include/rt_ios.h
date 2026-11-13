#ifndef RT_IOS_H
#define RT_IOS_H

#if defined(__APPLE__) || defined(__TARGET_OS_IPHONE)
#include <stdint.h>

typedef struct {
    float x;
    float y;
    int phase; // 0: began, 1: moved, 2: ended, 3: cancelled
} NyxiOSTouchEvent;

void nyx_ios_init(void* view_ptr);
void nyx_ios_render_frame(void);
NyxiOSTouchEvent nyx_ios_get_last_touch(void);
#endif

#endif // RT_IOS_H
