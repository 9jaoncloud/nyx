#include "rt_concurrency.h"
#include "rt_thread.h"
#include <stdlib.h>

NyxResult channel_new(void) {
    nyx_channel_t* ch = nyx_channel_create(64);
    NyxResult r;
    r.tag = RESULT_OK;
    r.data.ptr_val = (void*)ch;
    return r;
}

void channel_send(Channel* ch, void* msg) {
    nyx_channel_send((nyx_channel_t*)ch, msg);
}

NyxOption channel_recv(Channel* ch) {
    void* data = nyx_channel_recv((nyx_channel_t*)ch);
    NyxOption o;
    if (data) {
        o.tag = OPTION_SOME;
        o.data.ptr_val = data;
    } else {
        o.tag = OPTION_NONE_TAG;
    }
    return o;
}

NyxResult mutex_new(void) {
    nyx_mutex_t m = nyx_mutex_create();
    NyxResult r;
    r.tag = RESULT_OK;
    r.data.ptr_val = m;
    return r;
}

void mutex_lock(Mutex* m) {
    nyx_mutex_lock((nyx_mutex_t)m);
}

void mutex_unlock(Mutex* m) {
    nyx_mutex_unlock((nyx_mutex_t)m);
}

NyxResult spawn_thread(void* fn, void* arg) {
    nyx_thread_t t = nyx_thread_create((void (*)(void*))fn, arg);
    nyx_thread_join(t);
    NyxResult r;
    r.tag = RESULT_OK;
    r.data.int_val = 0;
    return r;
}
