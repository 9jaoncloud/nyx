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

typedef struct {
    Pcap handle;
} PcapSession;

NyxOption PcapSession_open(rt_string_t, int64_t, int, int64_t);
void PcapSession_close(PcapSession*);
int64_t PcapSession_send(PcapSession*, NyxVec);
// module sec_net
// import prelude::*
// extern nyx_pcap_open_live
// extern nyx_pcap_close
// extern nyx_pcap_next_ex
// extern nyx_pcap_sendpacket
// extern nyx_raw_socket
// extern nyx_raw_sendto
// extern nyx_raw_recvfrom
NyxOption PcapSession_open(rt_string_t device, int64_t snaplen, int promisc, int64_t to_ms) {
rt_string_t errbuf = rt_string_from("");
void* h = nyx_pcap_open_live(device, snaplen, ({ int64_t __if_res_1; if (promisc) {
__if_res_1 = 1LL;
} else {
__if_res_1 = 0LL;
}
__if_res_1; }), to_ms, errbuf);
if ((h == 0LL)) {
return rt_option_none();
} else {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ PcapSession _stmp_ = (PcapSession){.handle = h}; &_stmp_; }); _tmp; });
}
}

void PcapSession_close(PcapSession* self) {
nyx_pcap_close(self.handle);
}

int64_t PcapSession_send(PcapSession* self, NyxVec data) {
return nyx_pcap_sendpacket((*self).handle, data, (int64_t)(data.len));
}

