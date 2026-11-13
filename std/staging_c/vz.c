#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "runtime/rt_arena.h"
#include "runtime/rt_string.h"
#include "runtime/rt_io.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_option.h"
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include "runtime/rt_net.h"
#include "runtime/rt_crypto.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_time.h"
#include "runtime/rt_nursery.h"
#include "runtime/rt_collections.h"
#include "runtime/rt_math.h"
#include "runtime/rt_format.h"
#include "runtime/rt_thread.h"
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#ifndef NYX_TUPLE_DEFS
#define NYX_TUPLE_DEFS
typedef struct { void* f0; void* f1; } NyxTuple2;
typedef struct { void* f0; void* f1; void* f2; } NyxTuple3;
typedef struct { void* f0; void* f1; void* f2; void* f3; } NyxTuple4;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; } NyxTuple5;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; } NyxTuple6;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; } NyxTuple7;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; void* f7; } NyxTuple8;
#endif

#define TAG_VMState_Stopped 0
#define TAG_VMState_Configured 1
#define TAG_VMState_Booting 2
#define TAG_VMState_Running 3
#define TAG_VMState_Paused 4
#define TAG_VMState_Error 5
typedef struct {
    int tag;
    union {
    } data;
} VMState;

typedef struct {
    int64_t cpu_count;
    int64_t memory_bytes;
    rt_string_t disk_image_path;
    rt_string_t kernelcache_path;
    rt_string_t trustcache_path;
    int enable_root_ssh;
    int enable_vnc_display;
    int64_t ssh_port;
    int64_t vnc_port;
} IOSVMConfig;

typedef struct {
    IOSVMConfig config;
    VMState state;
    int64_t uptime_ms;
    int64_t instructions_executed;
    double cpu_usage_pct;
    rt_string_t ios_build_version;
    rt_string_t kernel_version;
    int64_t trustcache_entries;
} VirtualMachine;

IOSVMConfig IOSVMConfig_nyx_default();
VirtualMachine VirtualMachine_create(IOSVMConfig);
int VirtualMachine_start(VirtualMachine*);
int VirtualMachine_pause(VirtualMachine*);
int VirtualMachine_resume(VirtualMachine*);
int VirtualMachine_stop(VirtualMachine*);
rt_string_t VirtualMachine_exec_command(VirtualMachine*, rt_string_t);
// module vz
IOSVMConfig IOSVMConfig_default() {
return (IOSVMConfig){.cpu_count = 4LL, .memory_bytes = (((4LL * 1024LL) * 1024LL) * 1024LL), .disk_image_path = rt_string_from("ios_system_disk.raw"), .kernelcache_path = rt_string_from("kernelcache.patched"), .trustcache_path = rt_string_from("trustcache.bin"), .enable_root_ssh = 1, .enable_vnc_display = 1, .ssh_port = 2222LL, .vnc_port = 5900LL};
}

VirtualMachine VirtualMachine_create(IOSVMConfig config) {
return (VirtualMachine){.config = config, .state = (VMState){ .tag = TAG_VMState_Configured }, .uptime_ms = 0LL, .instructions_executed = 0LL, .cpu_usage_pct = 0, .ios_build_version = rt_string_from("iOS 27.1 (Build 24B83)"), .kernel_version = rt_string_from("Darwin Kernel 26.0.0 (xnu-11215.1.4~2/RELEASE_ARM64_T8122)"), .trustcache_entries = 142LL};
}

int VirtualMachine_start(VirtualMachine* self) {
(self->state = (VMState){ .tag = TAG_VMState_Running });
(self->cpu_usage_pct = 14.5);
(self->instructions_executed = ((*self).instructions_executed + 1000000LL));
return 1;
}

int VirtualMachine_pause(VirtualMachine* self) {
(self->state = (VMState){ .tag = TAG_VMState_Paused });
return 1;
}

int VirtualMachine_resume(VirtualMachine* self) {
(self->state = (VMState){ .tag = TAG_VMState_Running });
return 1;
}

int VirtualMachine_stop(VirtualMachine* self) {
(self->state = (VMState){ .tag = TAG_VMState_Stopped });
(self->cpu_usage_pct = 0);
return 1;
}

rt_string_t VirtualMachine_exec_command(VirtualMachine* self, rt_string_t cmd) {
({ if (rt_string_eq(cmd, rt_string_from("uname -a"))) {
return rt_string_from("Darwin iPhone-Research-VM 26.0.0 arm64\n");
} else if (rt_string_eq(cmd, rt_string_from("id"))) {
return rt_string_from("uid=0(root) gid=0(wheel)\n");
}
});
return rt_string_from("[Executed in Guest VM: OK]\n");
}

