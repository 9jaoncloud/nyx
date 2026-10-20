#ifndef RT_HTTP_H
#define RT_HTTP_H

#include "rt_string.h"
#include "rt_option.h"
#include "rt_vec.h"
#include "rt_map.h"
#include "rt_arena.h"
#include <stdint.h>
#include <stdbool.h>

#define NYX_HTTP_MAX_ROUTES 64
#define NYX_HTTP_BUFFER_SIZE 8192

typedef struct HttpRequest {
    rt_string_t method;
    rt_string_t path;
    rt_string_t query;
    rt_string_t body;
    rt_string_t auth_token;
    NyxMap headers;
} HttpRequest;

typedef struct HttpResponse {
    int64_t status;
    rt_string_t content_type;
    rt_string_t body;
    NyxMap headers;
} HttpResponse;

typedef HttpResponse (*NyxHttpHandler)(HttpRequest* req);

typedef struct RouteEntry {
    char method[16];
    char path[128];
    NyxHttpHandler handler;
} RouteEntry;

typedef struct Router {
    RouteEntry routes[NYX_HTTP_MAX_ROUTES];
    int route_count;
    bool enable_cors;
    bool enable_logging;
    char static_dir[256];
} Router;

/* ── HTTP Framework API ─────────────────────────────────────────────────── */
NyxResult new_router(void);
void router_get(Router* r, rt_string_t path, void* handler);
void router_post(Router* r, rt_string_t path, void* handler);
void router_put(Router* r, rt_string_t path, void* handler);
void router_delete(Router* r, rt_string_t path, void* handler);
void router_enable_cors(Router* r, bool enable);
void router_set_static_dir(Router* r, rt_string_t dir);

/* Response Builders */
HttpResponse http_response_json(int64_t status, rt_string_t json_body);
HttpResponse http_response_html(int64_t status, rt_string_t html_body);
HttpResponse http_response_text(int64_t status, rt_string_t text_body);

/* Dispatch and Server Loop */
HttpResponse router_dispatch(Router* r, const char* raw_request);
HttpResponse router_dispatch_slice(Router* r, NyxFatSlice raw_request_slice);
void serve(rt_string_t addr, Router* r);
void serve_single_request(Router* r, const char* mock_req_buf, char* resp_out_buf, size_t max_out);
void serve_single_request_slice(Router* r, NyxFatSlice mock_req_slice, NyxFatSlice resp_out_slice);

#endif // RT_HTTP_H
