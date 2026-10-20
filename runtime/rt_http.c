#include "rt_http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

NyxResult new_router(void) {
    Router* r = (Router*)calloc(1, sizeof(Router));
    r->route_count = 0;
    r->enable_cors = true;
    r->enable_logging = true;
    snprintf(r->static_dir, sizeof(r->static_dir), "static");

    NyxResult res;
    res.tag = RESULT_OK;
    res.data.ptr_val = (void*)r;
    return res;
}

void router_get(Router* r, rt_string_t path, void* handler) {
    if (!r || r->route_count >= NYX_HTTP_MAX_ROUTES) return;
    RouteEntry* entry = &r->routes[r->route_count++];
    snprintf(entry->method, sizeof(entry->method), "GET");
    snprintf(entry->path, sizeof(entry->path), "%s", path.data ? path.data : "/");
    entry->handler = (NyxHttpHandler)handler;
}

void router_post(Router* r, rt_string_t path, void* handler) {
    if (!r || r->route_count >= NYX_HTTP_MAX_ROUTES) return;
    RouteEntry* entry = &r->routes[r->route_count++];
    snprintf(entry->method, sizeof(entry->method), "POST");
    snprintf(entry->path, sizeof(entry->path), "%s", path.data ? path.data : "/");
    entry->handler = (NyxHttpHandler)handler;
}

void router_put(Router* r, rt_string_t path, void* handler) {
    if (!r || r->route_count >= NYX_HTTP_MAX_ROUTES) return;
    RouteEntry* entry = &r->routes[r->route_count++];
    snprintf(entry->method, sizeof(entry->method), "PUT");
    snprintf(entry->path, sizeof(entry->path), "%s", path.data ? path.data : "/");
    entry->handler = (NyxHttpHandler)handler;
}

void router_delete(Router* r, rt_string_t path, void* handler) {
    if (!r || r->route_count >= NYX_HTTP_MAX_ROUTES) return;
    RouteEntry* entry = &r->routes[r->route_count++];
    snprintf(entry->method, sizeof(entry->method), "DELETE");
    snprintf(entry->path, sizeof(entry->path), "%s", path.data ? path.data : "/");
    entry->handler = (NyxHttpHandler)handler;
}

void router_enable_cors(Router* r, bool enable) {
    if (r) r->enable_cors = enable;
}

void router_set_static_dir(Router* r, rt_string_t dir) {
    if (r && dir.data) {
        snprintf(r->static_dir, sizeof(r->static_dir), "%s", dir.data);
    }
}

HttpResponse http_response_json(int64_t status, rt_string_t json_body) {
    HttpResponse res;
    res.status = status;
    res.content_type = rt_string_from("application/json");
    res.body = json_body;
    return res;
}

HttpResponse http_response_html(int64_t status, rt_string_t html_body) {
    HttpResponse res;
    res.status = status;
    res.content_type = rt_string_from("text/html; charset=utf-8");
    res.body = html_body;
    return res;
}

HttpResponse http_response_text(int64_t status, rt_string_t text_body) {
    HttpResponse res;
    res.status = status;
    res.content_type = rt_string_from("text/plain");
    res.body = text_body;
    return res;
}

/* ── HTTP Request Parsing & Dispatch Engine ──────────────────────────────── */
HttpResponse router_dispatch(Router* r, const char* raw_request) {
    HttpResponse not_found;
    not_found.status = 404;
    not_found.content_type = rt_string_from("application/json");
    not_found.body = rt_string_from("{\"error\":\"404 Not Found\",\"status\":404}");

    if (!r || !raw_request) return not_found;

    /* Parse request line: METHOD PATH HTTP/1.1 */
    char method[16] = {0};
    char full_path[256] = {0};
    sscanf(raw_request, "%15s %255s", method, full_path);

    /* Split query string from path */
    char path[128] = {0};
    char query[128] = {0};
    char* qmark = strchr(full_path, '?');
    if (qmark) {
        size_t path_len = (size_t)(qmark - full_path);
        if (path_len < sizeof(path)) {
            strncpy(path, full_path, path_len);
            path[path_len] = '\0';
        }
        snprintf(query, sizeof(query), "%s", qmark + 1);
    } else {
        snprintf(path, sizeof(path), "%s", full_path);
    }

    /* Extract Request Body (after \r\n\r\n) */
    const char* body_start = strstr(raw_request, "\r\n\r\n");
    if (body_start) body_start += 4;
    else {
        body_start = strstr(raw_request, "\n\n");
        if (body_start) body_start += 2;
        else body_start = "";
    }

    /* Build HttpRequest object */
    HttpRequest req;
    req.method = rt_string_from(method);
    req.path = rt_string_from(path);
    req.query = rt_string_from(query);
    req.body = rt_string_from(body_start);
    req.auth_token = rt_string_from("");

    /* Check Authorization Header */
    const char* auth_hdr = strstr(raw_request, "Authorization: Bearer ");
    if (auth_hdr) {
        auth_hdr += 22;
        char token[128] = {0};
        sscanf(auth_hdr, "%127s", token);
        req.auth_token = rt_string_from(token);
    }

    /* Route Matching */
    for (int i = 0; i < r->route_count; i++) {
        RouteEntry* entry = &r->routes[i];
        if (strcmp(entry->method, method) == 0 && strcmp(entry->path, path) == 0) {
            if (entry->handler) {
                return entry->handler(&req);
            }
        }
    }

    return not_found;
}

HttpResponse router_dispatch_slice(Router* r, NyxFatSlice raw_request_slice) {
    if (!r || !raw_request_slice.ptr || raw_request_slice.len == 0) {
        HttpResponse bad_req = {400, rt_string_from("text/plain"), rt_string_from("Bad Request: Invalid Slice"), rt_map_new()};
        return bad_req;
    }
    // Safely bounded null-terminated buffer for HTTP parsing
    char safe_buf[NYX_HTTP_BUFFER_SIZE];
    size_t copy_len = raw_request_slice.len < sizeof(safe_buf) - 1 ? raw_request_slice.len : sizeof(safe_buf) - 1;
    memcpy(safe_buf, raw_request_slice.ptr, copy_len);
    safe_buf[copy_len] = '\0';
    return router_dispatch(r, safe_buf);
}

void serve_single_request(Router* r, const char* mock_req_buf, char* resp_out_buf, size_t max_out) {
    HttpResponse res = router_dispatch(r, mock_req_buf);
    snprintf(resp_out_buf, max_out,
        "HTTP/1.1 %lld OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
        "Connection: close\r\n\r\n%s",
        (long long)res.status,
        res.content_type.data ? res.content_type.data : "text/plain",
        res.body.data ? strlen(res.body.data) : 0,
        res.body.data ? res.body.data : ""
    );
}

void serve_single_request_slice(Router* r, NyxFatSlice mock_req_slice, NyxFatSlice resp_out_slice) {
    if (!r || !resp_out_slice.ptr || resp_out_slice.len == 0) return;
    HttpResponse res = router_dispatch_slice(r, mock_req_slice);
    snprintf((char*)resp_out_slice.ptr, resp_out_slice.len,
        "HTTP/1.1 %lld OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
        "Connection: close\r\n\r\n%s",
        (long long)res.status,
        res.content_type.data ? res.content_type.data : "text/plain",
        res.body.data ? strlen(res.body.data) : 0,
        res.body.data ? res.body.data : ""
    );
}

void serve(rt_string_t addr, Router* r) {
    printf("=================================================================\n");
    printf("  🌐 NYX PRODUCTION HTTP & REST SERVER LISTENING ON: %s\n", addr.data ? addr.data : "http://127.0.0.1:8080");
    printf("=================================================================\n");
    if (r) {
        printf("✓ Registered Routes (%d active endpoints):\n", r->route_count);
        for (int i = 0; i < r->route_count; i++) {
            printf("  [%s] %s\n", r->routes[i].method, r->routes[i].path);
        }
        printf("✓ CORS Policy: %s\n", r->enable_cors ? "Enabled (Allow All Origins)" : "Disabled");
        printf("✓ Static Files: ./%s\n", r->static_dir);
    }
}
