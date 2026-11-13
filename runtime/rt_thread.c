#include "rt_thread.h"
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
typedef HANDLE thread_handle;
typedef CRITICAL_SECTION mutex_handle;
typedef CONDITION_VARIABLE cond_handle;
#else
#include <pthread.h>
typedef pthread_t thread_handle;
typedef pthread_mutex_t mutex_handle;
typedef pthread_cond_t cond_handle;
#endif

/* Thread */
nyx_thread_t nyx_thread_create(void (*func)(void*), void* arg) {
#ifdef _WIN32
    thread_handle h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, arg, 0, NULL);
    return (nyx_thread_t)h;
#else
    thread_handle* h = (thread_handle*)malloc(sizeof(thread_handle));
    pthread_create(h, NULL, (void*(*)(void*))func, arg);
    return (nyx_thread_t)h;
#endif
}

void nyx_thread_join(nyx_thread_t thread) {
#ifdef _WIN32
    WaitForSingleObject((HANDLE)thread, INFINITE);
    CloseHandle((HANDLE)thread);
#else
    pthread_join(*(pthread_t*)thread, NULL);
    free(thread);
#endif
}

/* Mutex */
nyx_mutex_t nyx_mutex_create(void) {
    mutex_handle* m = (mutex_handle*)malloc(sizeof(mutex_handle));
#ifdef _WIN32
    InitializeCriticalSection(m);
#else
    pthread_mutex_init(m, NULL);
#endif
    return (nyx_mutex_t)m;
}

void nyx_mutex_lock(nyx_mutex_t m) {
#ifdef _WIN32
    EnterCriticalSection((CRITICAL_SECTION*)m);
#else
    pthread_mutex_lock((pthread_mutex_t*)m);
#endif
}

void nyx_mutex_unlock(nyx_mutex_t m) {
#ifdef _WIN32
    LeaveCriticalSection((CRITICAL_SECTION*)m);
#else
    pthread_mutex_unlock((pthread_mutex_t*)m);
#endif
}

void nyx_mutex_destroy(nyx_mutex_t m) {
#ifdef _WIN32
    DeleteCriticalSection((CRITICAL_SECTION*)m);
#else
    pthread_mutex_destroy((pthread_mutex_t*)m);
#endif
    free(m);
}

/* Condition Variable */
nyx_cond_t nyx_cond_create(void) {
    cond_handle* c = (cond_handle*)malloc(sizeof(cond_handle));
#ifdef _WIN32
    InitializeConditionVariable(c);
#else
    pthread_cond_init(c, NULL);
#endif
    return (nyx_cond_t)c;
}

void nyx_cond_wait(nyx_cond_t c, nyx_mutex_t m) {
#ifdef _WIN32
    SleepConditionVariableCS((CONDITION_VARIABLE*)c, (CRITICAL_SECTION*)m, INFINITE);
#else
    pthread_cond_wait((pthread_cond_t*)c, (pthread_mutex_t*)m);
#endif
}

void nyx_cond_signal(nyx_cond_t c) {
#ifdef _WIN32
    WakeConditionVariable((CONDITION_VARIABLE*)c);
#else
    pthread_cond_signal((pthread_cond_t*)c);
#endif
}

void nyx_cond_broadcast(nyx_cond_t c) {
#ifdef _WIN32
    WakeAllConditionVariable((CONDITION_VARIABLE*)c);
#else
    pthread_cond_broadcast((pthread_cond_t*)c);
#endif
}

void nyx_cond_destroy(nyx_cond_t c) {
#ifdef _WIN32
    /* CONDITION_VARIABLE doesn't need destroy */
#else
    pthread_cond_destroy((pthread_cond_t*)c);
#endif
    free(c);
}

/* Channel */
nyx_channel_t* nyx_channel_create(uint64_t capacity) {
    nyx_channel_t* ch = (nyx_channel_t*)calloc(1, sizeof(nyx_channel_t));
    ch->cap = capacity > 0 ? capacity : 64;
    ch->data = malloc(sizeof(void*) * ch->cap);
    ch->head = 0;
    ch->tail = 0;
    ch->closed = 0;
    ch->lock = nyx_mutex_create();
    ch->not_empty = nyx_cond_create();
    ch->not_full = nyx_cond_create();
    return ch;
}

void nyx_channel_send(nyx_channel_t* ch, void* data) {
    nyx_mutex_lock(ch->lock);
    while ((ch->tail - ch->head) >= ch->cap && !ch->closed) {
        nyx_cond_wait(ch->not_full, ch->lock);
    }
    if (ch->closed) {
        nyx_mutex_unlock(ch->lock);
        return;
    }
    ((void**)ch->data)[ch->tail % ch->cap] = data;
    ch->tail++;
    nyx_cond_signal(ch->not_empty);
    nyx_mutex_unlock(ch->lock);
}

void* nyx_channel_recv(nyx_channel_t* ch) {
    nyx_mutex_lock(ch->lock);
    while ((ch->tail - ch->head) == 0 && !ch->closed) {
        nyx_cond_wait(ch->not_empty, ch->lock);
    }
    if ((ch->tail - ch->head) == 0 && ch->closed) {
        nyx_mutex_unlock(ch->lock);
        return NULL;
    }
    void* data = ((void**)ch->data)[ch->head % ch->cap];
    ch->head++;
    nyx_cond_signal(ch->not_full);
    nyx_mutex_unlock(ch->lock);
    return data;
}

void nyx_channel_close(nyx_channel_t* ch) {
    nyx_mutex_lock(ch->lock);
    ch->closed = 1;
    nyx_cond_broadcast(ch->not_empty);
    nyx_cond_broadcast(ch->not_full);
    nyx_mutex_unlock(ch->lock);
}

void nyx_channel_destroy(nyx_channel_t* ch) {
    nyx_channel_close(ch);
    nyx_cond_destroy(ch->not_empty);
    nyx_cond_destroy(ch->not_full);
    nyx_mutex_destroy(ch->lock);
    free(ch->data);
    free(ch);
}
