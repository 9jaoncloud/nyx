#ifndef RT_THREAD_H
#define RT_THREAD_H
#include <stdint.h>

typedef void* nyx_thread_t;
typedef void* nyx_mutex_t;
typedef void* nyx_cond_t;

typedef struct {
    void* data;
    uint64_t cap;
    uint64_t head;
    uint64_t tail;
    int closed;
    nyx_mutex_t lock;
    nyx_cond_t not_empty;
    nyx_cond_t not_full;
} nyx_channel_t;

nyx_thread_t nyx_thread_create(void (*func)(void*), void* arg);
void nyx_thread_join(nyx_thread_t thread);

nyx_mutex_t nyx_mutex_create(void);
void nyx_mutex_lock(nyx_mutex_t m);
void nyx_mutex_unlock(nyx_mutex_t m);
void nyx_mutex_destroy(nyx_mutex_t m);

nyx_cond_t nyx_cond_create(void);
void nyx_cond_wait(nyx_cond_t c, nyx_mutex_t m);
void nyx_cond_signal(nyx_cond_t c);
void nyx_cond_broadcast(nyx_cond_t c);
void nyx_cond_destroy(nyx_cond_t c);

nyx_channel_t* nyx_channel_create(uint64_t capacity);
void nyx_channel_send(nyx_channel_t* ch, void* data);
void* nyx_channel_recv(nyx_channel_t* ch);
void nyx_channel_close(nyx_channel_t* ch);
void nyx_channel_destroy(nyx_channel_t* ch);

#endif
