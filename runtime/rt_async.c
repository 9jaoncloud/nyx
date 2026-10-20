#include "rt_async.h"
#include <stdlib.h>

NyxFuture* nyx_future_new(NyxFutureResumeFn resume, void* env) {
    NyxFuture* f = (NyxFuture*)calloc(1, sizeof(NyxFuture));
    f->state = NYX_FUTURE_PENDING;
    f->result.value = NULL;
    f->env = env;
    f->resume = resume;
    return f;
}

void nyx_future_complete(NyxFuture* f, void* value) {
    f->state = NYX_FUTURE_READY;
    f->result.value = value;
}

void nyx_future_error(NyxFuture* f, int code, const char* msg) {
    f->state = NYX_FUTURE_ERROR;
    f->result.error.code = code;
    f->result.error.msg = msg;
}

NyxEventLoop* nyx_event_loop_new(void) {
    NyxEventLoop* loop = (NyxEventLoop*)calloc(1, sizeof(NyxEventLoop));
    loop->capacity = 256;
    loop->tasks = (NyxFuture**)calloc(loop->capacity, sizeof(NyxFuture*));
    loop->count = 0;
    return loop;
}

void nyx_event_loop_free(NyxEventLoop* loop) {
    for (int i = 0; i < loop->count; i++) {
        free(loop->tasks[i]);
    }
    free(loop->tasks);
    free(loop);
}

void nyx_event_loop_submit(NyxEventLoop* loop, NyxFuture* future) {
    if (loop->count >= loop->capacity) {
        loop->capacity *= 2;
        loop->tasks = (NyxFuture**)realloc(loop->tasks, loop->capacity * sizeof(NyxFuture*));
    }
    loop->tasks[loop->count++] = future;
}

void nyx_event_loop_run(NyxEventLoop* loop) {
    while (1) {
        int all_done = 1;
        for (int i = 0; i < loop->count; i++) {
            NyxFuture* f = loop->tasks[i];
            if (f->state == NYX_FUTURE_PENDING) {
                all_done = 0;
                if (f->resume) {
                    f->resume(f);
                }
            }
        }
        if (all_done) break;
    }
}

void rt_yield(void) {
}

typedef void (*OldSchedulerFunc)(void*);

typedef struct {
    OldSchedulerFunc func;
    void* arg;
} OldTask;

typedef struct {
    OldTask tasks[256];
    int count;
    int current;
} OldScheduler;

Scheduler* rt_scheduler_new(int num_threads) {
    (void)num_threads;
    OldScheduler* s = (OldScheduler*)calloc(1, sizeof(OldScheduler));
    return (Scheduler*)s;
}

void rt_scheduler_spawn(Scheduler* s, void (*func)(void*), void* arg) {
    OldScheduler* os = (OldScheduler*)s;
    if (os->count >= 256) return;
    os->tasks[os->count].func = func;
    os->tasks[os->count].arg = arg;
    os->count++;
}

void rt_scheduler_run(Scheduler* s) {
    OldScheduler* os = (OldScheduler*)s;
    os->current = 0;
    while (os->current < os->count) {
        OldTask* t = &os->tasks[os->current];
        t->func(t->arg);
        os->current++;
    }
    free(s);
}
