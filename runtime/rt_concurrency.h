#ifndef RT_CONCURRENCY_H
#define RT_CONCURRENCY_H
#include "rt_string.h"
#include "rt_option.h"

typedef void* Channel;
typedef void* Mutex;

NyxResult channel_new(void);
void channel_send(Channel* ch, void* msg);
NyxOption channel_recv(Channel* ch);

NyxResult mutex_new(void);
void mutex_lock(Mutex* m);
void mutex_unlock(Mutex* m);

NyxResult spawn_thread(void* fn, void* arg);

#endif
