#ifndef RT_ASYNC_H
#define RT_ASYNC_H
#include <stdint.h>

typedef enum {
    NYX_FUTURE_PENDING = 0,
    NYX_FUTURE_READY = 1,
    NYX_FUTURE_ERROR = 2
} NyxFutureState;

typedef struct NyxFuture NyxFuture;
typedef void (*NyxFutureResumeFn)(NyxFuture*);

struct NyxFuture {
    NyxFutureState state;
    union {
        void* value;
        struct { int code; const char* msg; } error;
    } result;
    void* env;
    NyxFutureResumeFn resume;
};

typedef struct {
    NyxFuture** tasks;
    int count;
    int capacity;
} NyxEventLoop;

NyxEventLoop* nyx_event_loop_new(void);
void nyx_event_loop_free(NyxEventLoop* loop);
void nyx_event_loop_submit(NyxEventLoop* loop, NyxFuture* future);
void nyx_event_loop_run(NyxEventLoop* loop);

NyxFuture* nyx_future_new(NyxFutureResumeFn resume, void* env);
void nyx_future_complete(NyxFuture* f, void* value);
void nyx_future_error(NyxFuture* f, int code, const char* msg);

void rt_yield(void);

typedef struct Scheduler Scheduler;
Scheduler* rt_scheduler_new(int num_threads);
void rt_scheduler_spawn(Scheduler* s, void (*func)(void*), void* arg);
void rt_scheduler_run(Scheduler* s);

#endif
