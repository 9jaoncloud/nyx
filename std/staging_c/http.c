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

#define TAG_HttpError_Connection 0
#define TAG_HttpError_Timeout 1
#define TAG_HttpError_StatusCode 2
#define TAG_HttpError_InvalidUrl 3
#define TAG_HttpError_IoError 4
typedef struct {
    int tag;
    union {
        struct { rt_string_t f0; } Connection;
        struct { int64_t f0; rt_string_t f1; } StatusCode;
        struct { rt_string_t f0; } InvalidUrl;
        struct { rt_string_t f0; } IoError;
    } data;
} HttpError;

#define TAG_Method_GET 0
#define TAG_Method_POST 1
#define TAG_Method_PUT 2
#define TAG_Method_DELETE 3
#define TAG_Method_HEAD 4
#define TAG_Method_PATCH 5
#define TAG_Method_OPTIONS 6
typedef struct {
    int tag;
    union {
    } data;
} Method;

#define TAG_Body_Empty 0
#define TAG_Body_Text 1
#define TAG_Body_Json 2
#define TAG_Body_Bytes 3
typedef struct {
    int tag;
    union {
        struct { rt_string_t f0; } Text;
        struct { rt_string_t f0; } Json;
        struct { NyxVec f0; } Bytes;
    } data;
} Body;

typedef struct {
    Method method;
    rt_string_t url;
    NyxMap headers;
    NyxOption body;
    NyxMap params;
    NyxMap query_params;
} Request;

typedef struct {
    int64_t status;
    NyxMap headers;
    Body body;
} Response;

typedef struct {
    rt_string_t host;
    int64_t port;
    rt_string_t path;
    int use_tls;
} UrlParts;

typedef struct {
    int64_t timeout_ms;
} Client;

// runtime struct Router

typedef struct {
    Method method;
    rt_string_t path_pattern;
    rt_string_t handler_name;
} RouteEntry;

typedef struct {
    rt_string_t name;
    int active;
} Middleware;

// runtime struct Router

rt_string_t Method_to_string(Method*);
rt_string_t Request_param(Request*, rt_string_t);
rt_string_t Request_query(Request*, rt_string_t, rt_string_t);
NyxOption Request_header(Request*, rt_string_t);
NyxOption Request_bearer_token(Request*);
rt_string_t Body_to_string(Body*);
int Body_is_empty(Body*);
rt_string_t HttpError_to_string(HttpError*);
NyxResult parse_url(rt_string_t);
Client new_client();
Client new_client_with_timeout(int64_t);
NyxResult Client_get(Client*, rt_string_t);
NyxResult Client_get_with_headers(Client*, rt_string_t, NyxMap);
NyxResult Client_post(Client*, rt_string_t, rt_string_t);
NyxResult Client_post_json(Client*, rt_string_t, rt_string_t);
NyxResult Client_put(Client*, rt_string_t, rt_string_t);
NyxResult Client_delete(Client*, rt_string_t);
NyxResult Client_send(Client*, Request);
NyxResult parse_http_response(rt_string_t);
Router new_router();
void Router_route(Router*, Method, rt_string_t, Response (*)(Request));
void Router_get(Router*, rt_string_t, Response (*)(Request));
void Router_post(Router*, rt_string_t, Response (*)(Request));
void Router_put(Router*, rt_string_t, Response (*)(Request));
void Router_delete(Router*, rt_string_t, Response (*)(Request));
void serve(rt_string_t, Router);
NyxOption parse_http_request(rt_string_t);
Router Router_new();
Router Router_with_prefix(rt_string_t);
Router Router_use_middleware(Router*, rt_string_t);
Router Router_get(Router*, rt_string_t, rt_string_t);
Router Router_post(Router*, rt_string_t, rt_string_t);
Router Router_put(Router*, rt_string_t, rt_string_t);
Router Router_delete(Router*, rt_string_t, rt_string_t);
NyxOption Router_match_route(Router*, Method*, rt_string_t);
NyxOption match_path_pattern(rt_string_t, rt_string_t);
Response json_response(int64_t, rt_string_t);
Response html_response(int64_t, rt_string_t);
Response text_response(int64_t, rt_string_t);
Response redirect_response(rt_string_t);
// module http
// import prelude::*
// extern nyx_tcp_connect
// extern nyx_tcp_send
// extern nyx_tcp_recv
// extern nyx_tcp_close
// extern nyx_tcp_listen
// extern nyx_tcp_accept
// extern nyx_http_parse_status
// extern nyx_sleep_ms
// extern nyx_https_fetch
rt_string_t Method_to_string(Method* self) {
return ({
rt_string_t __match_result_0;
Method* __match_val_0 = self;
if ((*__match_val_0).tag == TAG_Method_GET) {
__match_result_0 = rt_string_from("GET");
}
 else if ((*__match_val_0).tag == TAG_Method_POST) {
__match_result_0 = rt_string_from("POST");
}
 else if ((*__match_val_0).tag == TAG_Method_PUT) {
__match_result_0 = rt_string_from("PUT");
}
 else if ((*__match_val_0).tag == TAG_Method_DELETE) {
__match_result_0 = rt_string_from("DELETE");
}
 else if ((*__match_val_0).tag == TAG_Method_HEAD) {
__match_result_0 = rt_string_from("HEAD");
}
 else if ((*__match_val_0).tag == TAG_Method_PATCH) {
__match_result_0 = rt_string_from("PATCH");
}
 else if ((*__match_val_0).tag == TAG_Method_OPTIONS) {
__match_result_0 = rt_string_from("OPTIONS");
}
__match_result_0;
});
}

rt_string_t Request_param(Request* self, rt_string_t name) {
return ({
rt_string_t __match_result_1;
NyxOption __match_val_1 = rt_map_get(&((*self).params), name);
if (__match_val_1.tag == OPTION_SOME) {
    rt_string_t val = rt_string_from((char*)__match_val_1.data.ptr_val);
__match_result_1 = ({ rt_string_t _c = val; rt_string_retain(&_c); _c; });
}
 else if (__match_val_1.tag == OPTION_NONE) {
__match_result_1 = rt_string_from("");
}
__match_result_1;
});
}

rt_string_t Request_query(Request* self, rt_string_t key, rt_string_t default_val) {
return ({
rt_string_t __match_result_2;
NyxOption __match_val_2 = rt_map_get(&((*self).query_params), key);
if (__match_val_2.tag == OPTION_SOME) {
    rt_string_t val = rt_string_from((char*)__match_val_2.data.ptr_val);
__match_result_2 = ({ rt_string_t _c = val; rt_string_retain(&_c); _c; });
}
 else if (__match_val_2.tag == OPTION_NONE) {
__match_result_2 = default_val;
}
__match_result_2;
});
}

NyxOption Request_header(Request* self, rt_string_t name) {
return rt_map_get(&((*self).headers), name);
}

NyxOption Request_bearer_token(Request* self) {
return ({
NyxOption __match_result_3;
NyxOption __match_val_3 = rt_map_get(&((*self).headers), rt_string_from("Authorization"));
if (__match_val_3.tag == OPTION_SOME) {
    void* auth = __match_val_3.data.ptr_val;
if (rt_string_starts_with(auth, rt_string_from("Bearer "))) {
__match_result_3 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = rt_string_slice(auth, 7LL, auth->len); &_stmp_; }); _tmp; });
} else {
__match_result_3 = rt_option_none();
}
}
 else if (__match_val_3.tag == OPTION_NONE) {
__match_result_3 = rt_option_none();
}
__match_result_3;
});
}

rt_string_t Body_to_string(Body* self) {
return ({
rt_string_t __match_result_4;
Body* __match_val_4 = self;
if ((*__match_val_4).tag == TAG_Body_Empty) {
__match_result_4 = rt_string_from("");
}
 else if ((*__match_val_4).tag == TAG_Body_Text) {
    rt_string_t s = (*__match_val_4).data.Text.f0;
__match_result_4 = ({ rt_string_t _c = s; rt_string_retain(&_c); _c; });
}
 else if ((*__match_val_4).tag == TAG_Body_Json) {
    rt_string_t s = (*__match_val_4).data.Json.f0;
__match_result_4 = ({ rt_string_t _c = s; rt_string_retain(&_c); _c; });
}
 else if ((*__match_val_4).tag == TAG_Body_Bytes) {
__match_result_4 = rt_string_from("<binary>");
}
__match_result_4;
});
}

int Body_is_empty(Body* self) {
return ({
int __match_result_5;
Body* __match_val_5 = self;
if ((*__match_val_5).tag == TAG_Body_Empty) {
__match_result_5 = 1;
}
 else {
__match_result_5 = 0;
}
__match_result_5;
});
}

rt_string_t HttpError_to_string(HttpError* self) {
return ({
rt_string_t __match_result_6;
HttpError* __match_val_6 = self;
if ((*__match_val_6).tag == TAG_HttpError_Connection) {
    rt_string_t msg = (*__match_val_6).data.Connection.f0;
__match_result_6 = rt_string_concat(rt_string_from("Connection error: "), ({ rt_string_t _c = msg; rt_string_retain(&_c); _c; }));
}
 else if ((*__match_val_6).tag == TAG_HttpError_Timeout) {
__match_result_6 = rt_string_from("Request timed out");
}
 else if ((*__match_val_6).tag == TAG_HttpError_StatusCode) {
    int64_t code = (*__match_val_6).data.StatusCode.f0;
    rt_string_t msg = (*__match_val_6).data.StatusCode.f1;
__match_result_6 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("HTTP "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)code); _buf.ref_count = 1; _buf; })), rt_string_from(": ")), ({ rt_string_t _c = msg; rt_string_retain(&_c); _c; }));
}
 else if ((*__match_val_6).tag == TAG_HttpError_InvalidUrl) {
    rt_string_t url = (*__match_val_6).data.InvalidUrl.f0;
__match_result_6 = rt_string_concat(rt_string_from("Invalid URL: "), ({ rt_string_t _c = url; rt_string_retain(&_c); _c; }));
}
 else if ((*__match_val_6).tag == TAG_HttpError_IoError) {
    rt_string_t msg = (*__match_val_6).data.IoError.f0;
__match_result_6 = rt_string_concat(rt_string_from("I/O error: "), ({ rt_string_t _c = msg; rt_string_retain(&_c); _c; }));
}
__match_result_6;
});
}

NyxResult parse_url(rt_string_t url) {
Arena* __arena = arena_create();
rt_string_t host = rt_string_from("");
rt_string_t path = rt_string_from("/");
int64_t port = 80LL;
int use_tls = 0;
rt_string_t rest = ({ rt_string_t __if_res_8; if (rt_string_starts_with(url, rt_string_from("https://"))) {
use_tls = 1;
port = 443LL;
__if_res_8 = rt_string_slice(url, 8LL, url.length);
} else if (rt_string_starts_with(url, rt_string_from("http://"))) {
__if_res_8 = rt_string_slice(url, 7LL, url.length);
} else {
__if_res_8 = ({ rt_string_t _c = url; rt_string_retain(&_c); _c; });
}
__if_res_8; });
int64_t slash_pos = rt_string_find(rest, rt_string_from("/"));
({ rt_string_t __if_res_9; if ((slash_pos >= 0LL)) {
host = rt_string_slice(rest, 0LL, (int64_t)(slash_pos));
__if_res_9 = (path = rt_string_slice(rest, (int64_t)(slash_pos), rest.length));
} else {
__if_res_9 = (host = ({ rt_string_t _c = rest; rt_string_retain(&_c); _c; }));
}
__if_res_9; });
int64_t colon_pos = rt_string_find(host, rt_string_from(":"));
({ if ((colon_pos >= 0LL)) {
rt_string_t port_str = rt_string_slice(host, ((int64_t)(colon_pos) + 1LL), host.length);
port = rt_string_parse_int(port_str);
host = rt_string_slice(host, 0LL, (int64_t)(colon_pos));
}
});
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ UrlParts _stmp_ = (UrlParts){.host = host, .port = port, .path = path, .use_tls = use_tls}; &_stmp_; }); _tmp; });
}

Client new_client() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (Client){.timeout_ms = 30000LL};
}

Client new_client_with_timeout(int64_t timeout_ms) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (Client){.timeout_ms = timeout_ms};
}

NyxResult Client_get(Client* self, rt_string_t url) {
Request req = (Request){.method = (Method){ .tag = TAG_Method_GET }, .url = ({ rt_string_t _c = url; rt_string_retain(&_c); _c; }), .headers = rt_map_new(), .body = rt_option_none()};
return Client_send(self, req);
}

NyxResult Client_get_with_headers(Client* self, rt_string_t url, NyxMap headers) {
Request req = (Request){.method = (Method){ .tag = TAG_Method_GET }, .url = url, .headers = headers, .body = rt_option_none()};
return Client_send(self, req);
}

NyxResult Client_post(Client* self, rt_string_t url, rt_string_t body) {
NyxMap headers = rt_map_new();
rt_map_insert(&(headers), rt_string_from("Content-Type"), (int64_t)(({ rt_string_t* _mtmp = malloc(sizeof(rt_string_t)); *_mtmp = rt_string_from("application/json"); (int64_t)_mtmp; })));
Request req = (Request){.method = (Method){ .tag = TAG_Method_POST }, .url = url, .headers = headers, .body = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Body _stmp_ = (Body){ .tag = TAG_Body_Text, .data.Text = {.f0 = body} }; &_stmp_; }); _tmp; })};
return Client_send(self, req);
}

NyxResult Client_post_json(Client* self, rt_string_t url, rt_string_t json) {
NyxMap headers = rt_map_new();
rt_map_insert(&(headers), rt_string_from("Content-Type"), (int64_t)(({ rt_string_t* _mtmp = malloc(sizeof(rt_string_t)); *_mtmp = rt_string_from("application/json"); (int64_t)_mtmp; })));
Request req = (Request){.method = (Method){ .tag = TAG_Method_POST }, .url = url, .headers = headers, .body = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Body _stmp_ = (Body){ .tag = TAG_Body_Json, .data.Json = {.f0 = json} }; &_stmp_; }); _tmp; })};
return Client_send(self, req);
}

NyxResult Client_put(Client* self, rt_string_t url, rt_string_t body) {
NyxMap headers = rt_map_new();
rt_map_insert(&(headers), rt_string_from("Content-Type"), (int64_t)(({ rt_string_t* _mtmp = malloc(sizeof(rt_string_t)); *_mtmp = rt_string_from("application/json"); (int64_t)_mtmp; })));
Request req = (Request){.method = (Method){ .tag = TAG_Method_PUT }, .url = url, .headers = headers, .body = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Body _stmp_ = (Body){ .tag = TAG_Body_Text, .data.Text = {.f0 = body} }; &_stmp_; }); _tmp; })};
return Client_send(self, req);
}

NyxResult Client_delete(Client* self, rt_string_t url) {
Request req = (Request){.method = (Method){ .tag = TAG_Method_DELETE }, .url = url, .headers = rt_map_new(), .body = rt_option_none()};
return Client_send(self, req);
}

NyxResult Client_send(Client* self, Request req) {
UrlParts url_parts = ({ NyxResult _qres = parse_url(({ rt_string_t _c = req.url; rt_string_retain(&_c); _c; })); if (_qres.tag == RESULT_ERR) { return _qres; } *(UrlParts*)_qres.data.ptr_val; });
({ if (url_parts.use_tls) {
rt_string_t headers_str = rt_string_from("");
for (int64_t _idx_key_value = 0; _idx_key_value < rt_map_iter(&(req.headers)).len; _idx_key_value++) {
    NyxTuple2 _tup_key_value = *(NyxTuple2*)rt_vec_get(rt_map_iter(&(req.headers)), _idx_key_value);
    int64_t key = (int64_t)(_tup_key_value.f0);
    int64_t value = (int64_t)(_tup_key_value.f1);
headers_str = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(headers_str, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)key); _s.ref_count = 1; _s; })), rt_string_from(": ")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)value); _s.ref_count = 1; _s; })), rt_string_from("\r\n"));
}
;
rt_string_t body_str = rt_string_from("");
NyxResult body_len = ({
int64_t __match_result_11;
NyxOption __match_val_11 = req.body;
if (__match_val_11.tag == OPTION_SOME) {
    void* b = __match_val_11.data.ptr_val;
body_str = void_to_string(b);
__match_result_11 = (int64_t)(body_str.length);
}
 else if (__match_val_11.tag == OPTION_NONE) {
__match_result_11 = 0LL;
}
__match_result_11;
});
uint8_t resp_buf[65536LL] = { 0LL };
int64_t status_code = 0LL;
int64_t n = nyx_https_fetch(((const uint8_t*)(Method_to_string(req.method).data)), ((const uint8_t*)(req.url.data)), ((const uint8_t*)(headers_str.data)), ((const uint8_t*)(body_str.data)), body_len, (&(resp_buf[0LL])), 65536LL, (&(status_code)));
({ if ((n < 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ HttpError _stmp_ = (HttpError){ .tag = TAG_HttpError_Connection, .data.Connection = {.f0 = rt_string_concat(rt_string_from("HTTPS/TLS request failed for "), req.url)} }; &_stmp_; }); _tmp; });
}
});
rt_string_t resp_body = rt_string_from((const char*)(/* slice */ 0));
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Response _stmp_ = (Response){.status = (int64_t)(status_code), .headers = rt_map_new(), .body = (Body){ .tag = TAG_Body_Text, .data.Text = {.f0 = resp_body} }}; &_stmp_; }); _tmp; });
}
});
int64_t fd = nyx_tcp_connect(((const uint8_t*)(url_parts.host.data)), url_parts.port);
({ if ((fd < 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ HttpError _stmp_ = (HttpError){ .tag = TAG_HttpError_Connection, .data.Connection = {.f0 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("Failed to connect to "), url_parts.host), rt_string_from(":")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)url_parts.port); _buf.ref_count = 1; _buf; }))} }; &_stmp_; }); _tmp; });
}
});
rt_string_t req_str = rt_string_concat(rt_string_concat(rt_string_concat(Method_to_string(req.method), rt_string_from(" ")), url_parts.path), rt_string_from(" HTTP/1.1\r\n"));
req_str = rt_string_concat(rt_string_concat(rt_string_concat(req_str, rt_string_from("Host: ")), url_parts.host), rt_string_from("\r\n"));
req_str = rt_string_concat(req_str, rt_string_from("Connection: close\r\n"));
for (int64_t _idx_key_value = 0; _idx_key_value < rt_map_iter(&(req.headers)).len; _idx_key_value++) {
    NyxTuple2 _tup_key_value = *(NyxTuple2*)rt_vec_get(rt_map_iter(&(req.headers)), _idx_key_value);
    int64_t key = (int64_t)(_tup_key_value.f0);
    int64_t value = (int64_t)(_tup_key_value.f1);
req_str = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(req_str, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)key); _s.ref_count = 1; _s; })), rt_string_from(": ")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)value); _s.ref_count = 1; _s; })), rt_string_from("\r\n"));
}
;
({
int64_t __match_result_14;
NyxOption __match_val_14 = req.body;
if (__match_val_14.tag == OPTION_SOME) {
    void* b = __match_val_14.data.ptr_val;
NyxResult body_bytes = ({
NyxVec __match_result_15;
Body* __match_val_15 = b;
if ((*__match_val_15).tag == TAG_Body_Text) {
    rt_string_t s = (*__match_val_15).data.Text.f0;
__match_result_15 = rt_string_as_bytes(s);
}
 else if ((*__match_val_15).tag == TAG_Body_Json) {
    rt_string_t s = (*__match_val_15).data.Json.f0;
__match_result_15 = rt_string_as_bytes(s);
}
 else if ((*__match_val_15).tag == TAG_Body_Bytes) {
    NyxVec v = (*__match_val_15).data.Bytes.f0;
__match_result_15 = rt_vec_clone(v);
}
 else if ((*__match_val_15).tag == TAG_Body_Empty) {
__match_result_15 = rt_vec_new(sizeof(int64_t));
}
__match_result_15;
});
req_str = rt_string_concat(rt_string_concat(rt_string_concat(req_str, rt_string_from("Content-Length: ")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)body_bytes.len); _buf.ref_count = 1; _buf; })), rt_string_from("\r\n"));
req_str = rt_string_concat(req_str, rt_string_from("\r\n"));
nyx_tcp_send(fd, ((const uint8_t*)(req_str.data)), (int64_t)(req_str.length));
({ if ((body_bytes.len > 0LL)) {
nyx_tcp_send(fd, ((const uint8_t*)(body_bytes)), (int64_t)(body_bytes.len));
}
});
}
 else if (__match_val_14.tag == OPTION_NONE) {
req_str = rt_string_concat(req_str, rt_string_from("\r\n"));
__match_result_14 = nyx_tcp_send(fd, ((const uint8_t*)(req_str.data)), (int64_t)(req_str.length));
}
__match_result_14;
});
uint8_t resp_buf[65536LL] = { 0LL };
int64_t total_read = nyx_tcp_recv(fd, (&(resp_buf[0LL])), 65536LL);
nyx_tcp_close(fd);
({ if ((total_read <= 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ HttpError _stmp_ = (HttpError){ .tag = TAG_HttpError_IoError, .data.IoError = {.f0 = rt_string_from("No response received")} }; &_stmp_; }); _tmp; });
}
});
rt_string_t resp_str = rt_string_from((const char*)(/* slice */ 0));
return parse_http_response(resp_str);
}

NyxResult parse_http_response(rt_string_t raw) {
Arena* __arena = arena_create();
int64_t lines = rt_string_split(raw, rt_string_from("\r\n"));
({ if ((lines.len < 1LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ HttpError _stmp_ = (HttpError){ .tag = TAG_HttpError_IoError, .data.IoError = {.f0 = rt_string_from("Empty response")} }; &_stmp_; }); _tmp; });
}
});
int64_t status_line = lines[0LL];
int64_t space_pos = rt_string_find(status_line, rt_string_from(" "));
({ if ((space_pos < 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ HttpError _stmp_ = (HttpError){ .tag = TAG_HttpError_IoError, .data.IoError = {.f0 = rt_string_from("Invalid status line")} }; &_stmp_; }); _tmp; });
}
});
NyxVec status_str = rt_string_slice(status_line, ((int64_t)(space_pos) + 1LL), rt_string_find_from(status_line, rt_string_from(" "), ((int64_t)(space_pos) + 1LL)));
int64_t status = rt_string_parse_int(status_str);
NyxMap headers = rt_map_new();
int64_t body_start = 0LL;
for (int64_t i = 1LL; i < lines.len; i++) {
int64_t line = lines[i];
({ if ((line.len == 0LL)) {
body_start = (i + 1LL);
break;
}
});
int64_t colon = rt_string_find(line, rt_string_from(":"));
({ if ((colon >= 0LL)) {
NyxVec key = rt_string_slice(line, 0LL, (int64_t)(colon));
NyxVec value = rt_string_slice(line, ((int64_t)(colon) + 2LL), line.len);
rt_map_insert(&(headers), key, (int64_t)(({ NyxVec* _mtmp = malloc(sizeof(NyxVec)); *_mtmp = value; (int64_t)_mtmp; })));
}
});
}
;
rt_string_t body_str = rt_string_from("");
for (int64_t i = body_start; i < lines.len; i++) {
({ if ((body_str.length > 0LL)) {
body_str = rt_string_concat(body_str, rt_string_from("\r\n"));
}
});
body_str = rt_string_concat(body_str, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)lines[i]); _s.ref_count = 1; _s; }));
}
;
Body body = ({ Body __if_res_24; if ((body_str.length > 0LL)) {
__if_res_24 = (Body){ .tag = TAG_Body_Text, .data.Text = {.f0 = body_str} };
} else {
__if_res_24 = (Body){ .tag = TAG_Body_Empty };
}
__if_res_24; });
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Response _stmp_ = (Response){.status = status, .headers = headers, .body = body}; &_stmp_; }); _tmp; });
}

Router new_router() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (Router){.routes = rt_vec_new(sizeof(int64_t))};
}

void Router_route(Router* self, Method method, rt_string_t path, void* handler) {
rt_vec_push(&(*self).routes, ({ NyxTuple3 _tmp_24 = (int64_t)(({ NyxTuple3 _tuple_25 = {.f0 = (void*)({ Method _stmp_0 = method; &_stmp_0; }), .f1 = (void*)({ rt_string_t _stmp_1 = path; _stmp_1.data; }), .f2 = (void*)(handler) }; _tuple_25; })); &_tmp_24; }));
}

void Router_get(Router* self, rt_string_t path, void* handler) {
Router_route(self, (Method){ .tag = TAG_Method_GET }, path, handler);
}

void Router_post(Router* self, rt_string_t path, void* handler) {
Router_route(self, (Method){ .tag = TAG_Method_POST }, path, handler);
}

void Router_put(Router* self, rt_string_t path, void* handler) {
Router_route(self, (Method){ .tag = TAG_Method_PUT }, path, handler);
}

void Router_delete(Router* self, rt_string_t path, void* handler) {
Router_route(self, (Method){ .tag = TAG_Method_DELETE }, path, handler);
}

void serve(rt_string_t addr, Router router) {
Arena* __arena = arena_create();
int64_t listener_fd = nyx_tcp_listen(((const uint8_t*)(addr.data)), 80LL);
({ if ((listener_fd < 0LL)) {
fprintf(stderr, "%s\n", rt_string_concat(rt_string_from("Failed to bind to "), addr).data);
return;
}
});
printf("%s\n", rt_string_concat(rt_string_from("Server listening on "), addr).data);
while (1) {
int64_t client_fd = nyx_tcp_accept(listener_fd);
({ if ((client_fd < 0LL)) {
continue;
}
});
uint8_t buf[8192LL] = { 0LL };
int64_t n = nyx_tcp_recv(client_fd, (&(buf[0LL])), 8192LL);
({ if ((n <= 0LL)) {
nyx_tcp_close(client_fd);
continue;
}
});
rt_string_t request_str = rt_string_from((const char*)(/* slice */ 0));
NyxOption request = parse_http_request(request_str);
({
int64_t __match_result_29;
NyxOption __match_val_29 = request;
if (__match_val_29.tag == OPTION_SOME) {
    void* req = __match_val_29.data.ptr_val;
Response response = (Response){.status = 404LL, .headers = rt_map_new(), .body = (Body){ .tag = TAG_Body_Text, .data.Text = {.f0 = rt_string_from("Not Found")} }};
for (int64_t _idx_method_path_handler = 0; _idx_method_path_handler < rt_vec_iter(router.routes).len; _idx_method_path_handler++) {
    NyxTuple3 _tup_method_path_handler = *(NyxTuple3*)rt_vec_get(rt_vec_iter(router.routes), _idx_method_path_handler);
    rt_string_t method = *(rt_string_t*)(_tup_method_path_handler.f0);
    int64_t path = (int64_t)(_tup_method_path_handler.f1);
    int64_t handler = (int64_t)(_tup_method_path_handler.f2);
({ if (((method == (*req).method.tag) && rt_string_eq(path, (*req).url))) {
response = handler(req);
break;
}
});
}
;
rt_string_t resp_str = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("HTTP/1.1 "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)response.status); _buf.ref_count = 1; _buf; })), rt_string_from(" OK\r\nContent-Length: ")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)Body_to_string(response.body).length); _buf.ref_count = 1; _buf; })), rt_string_from("\r\n\r\n")), Body_to_string(response.body));
__match_result_29 = nyx_tcp_send(client_fd, ((const uint8_t*)(resp_str.data)), (int64_t)(resp_str.length));
}
 else if (__match_val_29.tag == OPTION_NONE) {
rt_string_t resp = rt_string_from("HTTP/1.1 400 Bad Request\r\nContent-Length: 15\r\n\r\nBad Request");
__match_result_29 = nyx_tcp_send(client_fd, ((const uint8_t*)(resp.data)), (int64_t)(resp.length));
}
__match_result_29;
});
nyx_tcp_close(client_fd);
}
;
arena_destroy(__arena);
}

NyxOption parse_http_request(rt_string_t raw) {
Arena* __arena = arena_create();
int64_t lines = rt_string_split(raw, rt_string_from("\r\n"));
({ if ((lines.len < 1LL)) {
return rt_option_none();
}
});
int64_t request_line = lines[0LL];
int64_t parts = int64_t_split(request_line, rt_string_from(" "));
({ if ((parts.len < 2LL)) {
return rt_option_none();
}
});
NyxOption method = ({
Method __match_result_33;
int64_t __match_val_33 = parts[0LL];
if (__match_val_33 == rt_string_from("GET")) {
__match_result_33 = (Method){ .tag = TAG_Method_GET };
}
 else if (__match_val_33 == rt_string_from("POST")) {
__match_result_33 = (Method){ .tag = TAG_Method_POST };
}
 else if (__match_val_33 == rt_string_from("PUT")) {
__match_result_33 = (Method){ .tag = TAG_Method_PUT };
}
 else if (__match_val_33 == rt_string_from("DELETE")) {
__match_result_33 = (Method){ .tag = TAG_Method_DELETE };
}
 else if (__match_val_33 == rt_string_from("HEAD")) {
__match_result_33 = (Method){ .tag = TAG_Method_HEAD };
}
 else if (__match_val_33 == rt_string_from("PATCH")) {
__match_result_33 = (Method){ .tag = TAG_Method_PATCH };
}
 else {
__match_result_33 = (Method){ .tag = TAG_Method_GET };
}
__match_result_33;
});
NyxMap headers = rt_map_new();
for (int64_t i = 1LL; i < lines.len; i++) {
int64_t line = lines[i];
({ if ((line.len == 0LL)) {
break;
}
});
int64_t colon = rt_string_find(line, rt_string_from(":"));
({ if ((colon >= 0LL)) {
NyxVec key = rt_string_slice(line, 0LL, (int64_t)(colon));
NyxVec value = rt_string_slice(line, ((int64_t)(colon) + 2LL), line.len);
rt_map_insert(&(headers), key, (int64_t)(({ NyxVec* _mtmp = malloc(sizeof(NyxVec)); *_mtmp = value; (int64_t)_mtmp; })));
}
});
}
;
rt_string_t raw_url = ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)parts[1LL]); _buf.ref_count = 1; _buf; });
rt_string_t clean_url = ({ rt_string_t _c = raw_url; rt_string_retain(&_c); _c; });
NyxMap query_params = rt_map_new();
int64_t qmark = rt_string_find(raw_url, rt_string_from("?"));
({ if ((qmark >= 0LL)) {
clean_url = rt_string_slice(raw_url, 0LL, (int64_t)(qmark));
rt_string_t query_str = rt_string_slice(raw_url, ((int64_t)(qmark) + 1LL), raw_url.length);
int64_t pairs = rt_string_split(query_str, rt_string_from("&"));
int64_t p = 0LL;
while ((p < pairs.len)) {
int64_t pair = pairs[p];
int64_t eq = rt_string_find(pair, rt_string_from("="));
({ if ((eq >= 0LL)) {
NyxVec k = rt_string_slice(pair, 0LL, (int64_t)(eq));
NyxVec v = rt_string_slice(pair, ((int64_t)(eq) + 1LL), pair.len);
rt_map_insert(&(query_params), k, (int64_t)(({ NyxVec* _mtmp = malloc(sizeof(NyxVec)); *_mtmp = v; (int64_t)_mtmp; })));
}
});
p = (p + 1LL);
}
;
}
});
arena_destroy(__arena);
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Request _stmp_ = (Request){.method = method, .url = clean_url, .headers = headers, .body = rt_option_none(), .params = rt_map_new(), .query_params = query_params}; &_stmp_; }); _tmp; });
}

Router Router_new() {
return (Router){.routes = rt_vec_new(sizeof(int64_t)), .middlewares = rt_vec_new(sizeof(int64_t)), .cors_enabled = 1, .prefix = rt_string_from("")};
}

Router Router_with_prefix(rt_string_t prefix) {
return (Router){.routes = rt_vec_new(sizeof(int64_t)), .middlewares = rt_vec_new(sizeof(int64_t)), .cors_enabled = 1, .prefix = prefix};
}

Router Router_use_middleware(Router* self, rt_string_t name) {
rt_vec_push(&(*self).middlewares, ({ Middleware _tmp_38 = (int64_t)((Middleware){.name = name, .active = 1}); &_tmp_38; }));
return self;
}

Router Router_get(Router* self, rt_string_t path, rt_string_t handler_name) {
rt_string_t full_path = rt_string_concat(({ rt_string_t _c = (*self).prefix; rt_string_retain(&_c); _c; }), path);
rt_vec_push(&(*self).routes, ({ RouteEntry _tmp_39 = (int64_t)((RouteEntry){.method = (Method){ .tag = TAG_Method_GET }, .path_pattern = full_path, .handler_name = handler_name}); &_tmp_39; }));
return self;
}

Router Router_post(Router* self, rt_string_t path, rt_string_t handler_name) {
rt_string_t full_path = rt_string_concat(({ rt_string_t _c = (*self).prefix; rt_string_retain(&_c); _c; }), path);
rt_vec_push(&(*self).routes, ({ RouteEntry _tmp_40 = (int64_t)((RouteEntry){.method = (Method){ .tag = TAG_Method_POST }, .path_pattern = full_path, .handler_name = handler_name}); &_tmp_40; }));
return self;
}

Router Router_put(Router* self, rt_string_t path, rt_string_t handler_name) {
rt_string_t full_path = rt_string_concat(({ rt_string_t _c = (*self).prefix; rt_string_retain(&_c); _c; }), path);
rt_vec_push(&(*self).routes, ({ RouteEntry _tmp_41 = (int64_t)((RouteEntry){.method = (Method){ .tag = TAG_Method_PUT }, .path_pattern = full_path, .handler_name = handler_name}); &_tmp_41; }));
return self;
}

Router Router_delete(Router* self, rt_string_t path, rt_string_t handler_name) {
rt_string_t full_path = rt_string_concat(({ rt_string_t _c = (*self).prefix; rt_string_retain(&_c); _c; }), path);
rt_vec_push(&(*self).routes, ({ RouteEntry _tmp_42 = (int64_t)((RouteEntry){.method = (Method){ .tag = TAG_Method_DELETE }, .path_pattern = full_path, .handler_name = handler_name}); &_tmp_42; }));
return self;
}

NyxOption Router_match_route(Router* self, Method* method, rt_string_t request_path) {
int64_t r = 0LL;
while ((r < (*self).routes.len)) {
int64_t entry = (&((*(int64_t*)rt_vec_get((*self).routes, r))));
({ if (rt_string_eq(Method_to_string(entry.method), Method_to_string(method))) {
NyxOption matched = match_path_pattern(({ rt_string_t _c = entry.path_pattern; rt_string_retain(&_c); _c; }), ({ rt_string_t _c = request_path; rt_string_retain(&_c); _c; }));
({
int64_t __match_result_44;
NyxOption __match_val_44 = matched;
if (__match_val_44.tag == OPTION_SOME) {
    void* params = __match_val_44.data.ptr_val;
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)(params); _tmp; });
}
 else if (__match_val_44.tag == OPTION_NONE) {
}
__match_result_44;
});
}
});
r = (r + 1LL);
}
;
return rt_option_none();
}

NyxOption match_path_pattern(rt_string_t pattern, rt_string_t actual) {
Arena* __arena = arena_create();
int64_t pat_parts = rt_string_split(pattern, rt_string_from("/"));
int64_t act_parts = rt_string_split(actual, rt_string_from("/"));
({ if ((pat_parts.len != act_parts.len)) {
return rt_option_none();
}
});
NyxMap params = rt_map_new();
int64_t i = 0LL;
while ((i < pat_parts.len)) {
int64_t p = pat_parts[i];
int64_t a = act_parts[i];
({ if (rt_string_starts_with(p, rt_string_from(":"))) {
NyxVec param_name = rt_string_slice(p, 1LL, p.len);
rt_map_insert(&(params), param_name, (int64_t)(a));
} else if ((p != a)) {
return rt_option_none();
}
});
i = (i + 1LL);
}
;
arena_destroy(__arena);
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ NyxMap _stmp_ = params; &_stmp_; }); _tmp; });
}

Response json_response(int64_t status, rt_string_t json_str) {
Arena* __arena = arena_create();
NyxMap headers = rt_map_new();
rt_map_insert(&(headers), rt_string_from("Content-Type"), (int64_t)(({ rt_string_t* _mtmp = malloc(sizeof(rt_string_t)); *_mtmp = rt_string_from("application/json"); (int64_t)_mtmp; })));
rt_map_insert(&(headers), rt_string_from("Access-Control-Allow-Origin"), (int64_t)(({ rt_string_t* _mtmp = malloc(sizeof(rt_string_t)); *_mtmp = rt_string_from("*"); (int64_t)_mtmp; })));
arena_destroy(__arena);
return (Response){.status = status, .headers = headers, .body = (Body){ .tag = TAG_Body_Json, .data.Json = {.f0 = json_str} }};
}

Response html_response(int64_t status, rt_string_t html_str) {
Arena* __arena = arena_create();
NyxMap headers = rt_map_new();
rt_map_insert(&(headers), rt_string_from("Content-Type"), (int64_t)(({ rt_string_t* _mtmp = malloc(sizeof(rt_string_t)); *_mtmp = rt_string_from("text/html; charset=utf-8"); (int64_t)_mtmp; })));
arena_destroy(__arena);
return (Response){.status = status, .headers = headers, .body = (Body){ .tag = TAG_Body_Text, .data.Text = {.f0 = html_str} }};
}

Response text_response(int64_t status, rt_string_t text) {
Arena* __arena = arena_create();
NyxMap headers = rt_map_new();
rt_map_insert(&(headers), rt_string_from("Content-Type"), (int64_t)(({ rt_string_t* _mtmp = malloc(sizeof(rt_string_t)); *_mtmp = rt_string_from("text/plain; charset=utf-8"); (int64_t)_mtmp; })));
arena_destroy(__arena);
return (Response){.status = status, .headers = headers, .body = (Body){ .tag = TAG_Body_Text, .data.Text = {.f0 = text} }};
}

Response redirect_response(rt_string_t location) {
Arena* __arena = arena_create();
NyxMap headers = rt_map_new();
rt_map_insert(&(headers), rt_string_from("Location"), (int64_t)(({ rt_string_t* _mtmp = malloc(sizeof(rt_string_t)); *_mtmp = location; (int64_t)_mtmp; })));
arena_destroy(__arena);
return (Response){.status = 302LL, .headers = headers, .body = (Body){ .tag = TAG_Body_Empty }};
}

