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

void* http_create_router(void) {
    NyxResult r = new_router();
    return r.data.ptr_val;
}

void router_get(void* r_ptr, rt_string_t path, void* handler) {
    Router* r = (Router*)r_ptr;
    if (!r || r->route_count >= NYX_HTTP_MAX_ROUTES) return;
    RouteEntry* entry = &r->routes[r->route_count++];
    snprintf(entry->method, sizeof(entry->method), "GET");
    snprintf(entry->path, sizeof(entry->path), "%s", path.data ? path.data : "/");
    entry->handler = (NyxHttpHandler)handler;
}

void router_post(void* r_ptr, rt_string_t path, void* handler) {
    Router* r = (Router*)r_ptr;
    if (!r || r->route_count >= NYX_HTTP_MAX_ROUTES) return;
    RouteEntry* entry = &r->routes[r->route_count++];
    snprintf(entry->method, sizeof(entry->method), "POST");
    snprintf(entry->path, sizeof(entry->path), "%s", path.data ? path.data : "/");
    entry->handler = (NyxHttpHandler)handler;
}

void router_put(void* r_ptr, rt_string_t path, void* handler) {
    Router* r = (Router*)r_ptr;
    if (!r || r->route_count >= NYX_HTTP_MAX_ROUTES) return;
    RouteEntry* entry = &r->routes[r->route_count++];
    snprintf(entry->method, sizeof(entry->method), "PUT");
    snprintf(entry->path, sizeof(entry->path), "%s", path.data ? path.data : "/");
    entry->handler = (NyxHttpHandler)handler;
}

void router_delete(void* r_ptr, rt_string_t path, void* handler) {
    Router* r = (Router*)r_ptr;
    if (!r || r->route_count >= NYX_HTTP_MAX_ROUTES) return;
    RouteEntry* entry = &r->routes[r->route_count++];
    snprintf(entry->method, sizeof(entry->method), "DELETE");
    snprintf(entry->path, sizeof(entry->path), "%s", path.data ? path.data : "/");
    entry->handler = (NyxHttpHandler)handler;
}

void router_enable_cors(void* r_ptr, int enable) {
    Router* r = (Router*)r_ptr;
    if (r) r->enable_cors = (enable != 0);
}

void router_set_static_dir(void* r_ptr, rt_string_t dir) {
    Router* r = (Router*)r_ptr;
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

#include <time.h>
#include <sys/stat.h>

#ifdef _WIN32
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>
  typedef SOCKET nyx_socket_t;
  #define NYX_INVALID_SOCKET INVALID_SOCKET
  #define nyx_close_sock(s) closesocket(s)
#else
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <fcntl.h>
  #include <errno.h>
  typedef int nyx_socket_t;
  #define NYX_INVALID_SOCKET (-1)
  #define nyx_close_sock(s) close(s)
#endif

static const char* http_status_text(int64_t status) {
    switch (status) {
        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 204: return "No Content";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 304: return "Not Modified";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 500: return "Internal Server Error";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        default: return "OK";
    }
}

static const char* get_mime_type(const char* path) {
    const char* dot = strrchr(path, '.');
    if (!dot) return "text/plain";
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0) return "text/html; charset=utf-8";
    if (strcmp(dot, ".css") == 0) return "text/css; charset=utf-8";
    if (strcmp(dot, ".js") == 0) return "application/javascript";
    if (strcmp(dot, ".json") == 0) return "application/json";
    if (strcmp(dot, ".svg") == 0) return "image/svg+xml";
    if (strcmp(dot, ".png") == 0) return "image/png";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(dot, ".wasm") == 0) return "application/wasm";
    if (strcmp(dot, ".ico") == 0) return "image/x-icon";
    return "application/octet-stream";
}

static void parse_listen_addr(const char* in_addr, char* out_host, size_t host_size, int* out_port) {
    *out_port = 8080;
    snprintf(out_host, host_size, "0.0.0.0");

    if (!in_addr || strlen(in_addr) == 0) {
        return;
    }

    const char* p = in_addr;
    if (strncmp(p, "http://", 7) == 0) p += 7;
    else if (strncmp(p, "https://", 8) == 0) p += 8;

    /* Check if pure port number like "8080" */
    bool all_digits = true;
    for (const char* c = p; *c; c++) {
        if (*c < '0' || *c > '9') {
            all_digits = false;
            break;
        }
    }
    if (all_digits && strlen(p) > 0) {
        *out_port = atoi(p);
        return;
    }

    /* Check if leading colon like ":8080" */
    if (*p == ':') {
        *out_port = atoi(p + 1);
        return;
    }

    /* Host and optional port like "127.0.0.1:8080" */
    const char* colon = strchr(p, ':');
    if (colon) {
        size_t hlen = (size_t)(colon - p);
        if (hlen >= host_size) hlen = host_size - 1;
        strncpy(out_host, p, hlen);
        out_host[hlen] = '\0';
        *out_port = atoi(colon + 1);
    } else {
        strncpy(out_host, p, host_size - 1);
        out_host[host_size - 1] = '\0';
    }

    if (strcmp(out_host, "localhost") == 0) {
        snprintf(out_host, host_size, "127.0.0.1");
    }
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

    /* CORS Preflight OPTIONS Check */
    if (strcmp(method, "OPTIONS") == 0 && r->enable_cors) {
        HttpResponse cors_res;
        cors_res.status = 204;
        cors_res.content_type = rt_string_from("text/plain");
        cors_res.body = rt_string_from("");
        return cors_res;
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

    /* Static File Fallback */
    if (r->static_dir[0] != '\0' && (strcmp(method, "GET") == 0 || strcmp(method, "HEAD") == 0)) {
        if (strstr(path, "..") == NULL) { /* Prevent directory traversal */
            char filepath[512];
            if (strcmp(path, "/") == 0) {
                snprintf(filepath, sizeof(filepath), "%s/index.html", r->static_dir);
            } else {
                const char* rel_path = (path[0] == '/') ? path + 1 : path;
                snprintf(filepath, sizeof(filepath), "%s/%s", r->static_dir, rel_path);
            }

            FILE* f = fopen(filepath, "rb");
            if (f) {
                fseek(f, 0, SEEK_END);
                long fsize = ftell(f);
                fseek(f, 0, SEEK_SET);
                if (fsize >= 0 && fsize < 10 * 1024 * 1024) {
                    char* fbuf = (char*)malloc(fsize + 1);
                    if (fbuf) {
                        size_t n = fread(fbuf, 1, fsize, f);
                        fbuf[n] = '\0';
                        fclose(f);

                        HttpResponse sres;
                        sres.status = 200;
                        sres.content_type = rt_string_from(get_mime_type(filepath));
                        sres.body = rt_string_from(fbuf);
                        free(fbuf);
                        return sres;
                    }
                }
                fclose(f);
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
    char safe_buf[NYX_HTTP_BUFFER_SIZE];
    size_t copy_len = raw_request_slice.len < sizeof(safe_buf) - 1 ? raw_request_slice.len : sizeof(safe_buf) - 1;
    memcpy(safe_buf, raw_request_slice.ptr, copy_len);
    safe_buf[copy_len] = '\0';
    return router_dispatch(r, safe_buf);
}

void serve_single_request(Router* r, const char* mock_req_buf, char* resp_out_buf, size_t max_out) {
    HttpResponse res = router_dispatch(r, mock_req_buf);
    const char* stext = http_status_text(res.status);
    snprintf(resp_out_buf, max_out,
        "HTTP/1.1 %lld %s\r\n"
        "Server: Nyx-WebServer/0.53.0 (Zero-GC Region Core)\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
        "Connection: close\r\n\r\n%s",
        (long long)res.status,
        stext,
        res.content_type.data ? res.content_type.data : "text/plain",
        res.body.data ? strlen(res.body.data) : 0,
        res.body.data ? res.body.data : ""
    );
}

void serve_single_request_slice(Router* r, NyxFatSlice mock_req_slice, NyxFatSlice resp_out_slice) {
    if (!r || !resp_out_slice.ptr || resp_out_slice.len == 0) return;
    HttpResponse res = router_dispatch_slice(r, mock_req_slice);
    const char* stext = http_status_text(res.status);
    snprintf((char*)resp_out_slice.ptr, resp_out_slice.len,
        "HTTP/1.1 %lld %s\r\n"
        "Server: Nyx-WebServer/0.53.0 (Zero-GC Region Core)\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
        "Connection: close\r\n\r\n%s",
        (long long)res.status,
        stext,
        res.content_type.data ? res.content_type.data : "text/plain",
        res.body.data ? strlen(res.body.data) : 0,
        res.body.data ? res.body.data : ""
    );
}

void serve_with_options(rt_string_t addr, void* r_ptr, int64_t max_requests) {
    Router* r = (Router*)r_ptr;
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    char host[128] = "127.0.0.1";
    int port = 8080;
    parse_listen_addr(addr.data, host, sizeof(host), &port);

    nyx_socket_t server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == NYX_INVALID_SOCKET) {
        fprintf(stderr, "❌ [Nyx Web Server] Failed to create TCP socket.\n");
        return;
    }

    int opt = 1;
#ifdef _WIN32
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
#else
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons((uint16_t)port);
    if (strcmp(host, "0.0.0.0") == 0 || strlen(host) == 0) {
        saddr.sin_addr.s_addr = INADDR_ANY;
    } else if (strcmp(host, "127.0.0.1") == 0 || strcmp(host, "localhost") == 0) {
        saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    } else {
        inet_pton(AF_INET, host, &saddr.sin_addr);
    }

    if (bind(server_fd, (struct sockaddr*)&saddr, sizeof(saddr)) != 0) {
        fprintf(stderr, "❌ [Nyx Web Server Error] Failed to bind to %s:%d (Port in use or access denied)\n", host, port);
        nyx_close_sock(server_fd);
        return;
    }

    if (listen(server_fd, 128) != 0) {
        fprintf(stderr, "❌ [Nyx Web Server Error] Failed to listen on socket %s:%d\n", host, port);
        nyx_close_sock(server_fd);
        return;
    }

    printf("=================================================================\n");
    printf("  🌐 NYX SOVEREIGN HIGH-PERFORMANCE HTTP/1.1 & REST SERVER ENGINE\n");
    printf("=================================================================\n");
    printf("  ✓ Active TCP Socket Bound: http://%s:%d\n", host, port);
    printf("  ✓ Core Memory Model: Zero-GC Region Bump Arenas\n");
    if (r) {
        printf("  ✓ Registered Endpoints (%d active routes):\n", r->route_count);
        for (int i = 0; i < r->route_count; i++) {
            printf("    [%-6s] %s\n", r->routes[i].method, r->routes[i].path);
        }
        printf("  ✓ CORS Policy: %s\n", r->enable_cors ? "Enabled (Allow All Origins)" : "Disabled");
        if (r->static_dir[0]) {
            printf("  ✓ Static File Root: ./%s\n", r->static_dir);
        }
    }
    printf("  ✓ Production Listener: ACTIVE & READY FOR TRAFFIC\n");
    printf("=================================================================\n");
    fflush(stdout);

    if (max_requests <= 0) {
        const char* env_max = getenv("NYX_HTTP_MAX_REQUESTS");
        if (env_max && atoi(env_max) > 0) {
            max_requests = atoi(env_max);
        }
    }

    int requests_served = 0;
    while (1) {
        if (max_requests > 0 && requests_served >= max_requests) {
            printf("  ✓ Nyx HTTP Server completed requested workload (%d requests). Shutting down cleanly.\n", max_requests);
            break;
        }

        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        nyx_socket_t client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd == NYX_INVALID_SOCKET) {
            continue;
        }

#ifdef _WIN32
        DWORD timeout = 5000;
        setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
        setsockopt(client_fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
#else
        struct timeval tv = {5, 0};
        setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
        setsockopt(client_fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));
#endif

        char req_buf[NYX_HTTP_BUFFER_SIZE * 2] = {0};
        int bytes_received = recv(client_fd, req_buf, sizeof(req_buf) - 1, 0);
        if (bytes_received <= 0) {
            nyx_close_sock(client_fd);
            continue;
        }
        req_buf[bytes_received] = '\0';

        if (strstr(req_buf, "GET /__nyx_shutdown__") != NULL) {
            const char* shut_resp =
                "HTTP/1.1 200 OK\r\n"
                "Server: Nyx-WebServer/0.53.0\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: 32\r\n"
                "Connection: close\r\n\r\n"
                "{\"status\":\"shutdown_initiated\"}";
            send(client_fd, shut_resp, (int)strlen(shut_resp), 0);
            nyx_close_sock(client_fd);
            printf("  [Nyx Web Server] Administrative shutdown initiated via /__nyx_shutdown__\n");
            break;
        }

        /* Handle Expect: 100-continue from .NET / Java clients */
        if (strstr(req_buf, "100-continue") != NULL) {
            const char* cont_resp = "HTTP/1.1 100 Continue\r\n\r\n";
            send(client_fd, cont_resp, (int)strlen(cont_resp), 0);
            int more_bytes = recv(client_fd, req_buf + bytes_received, (int)(sizeof(req_buf) - 1 - bytes_received), 0);
            if (more_bytes > 0) {
                bytes_received += more_bytes;
                req_buf[bytes_received] = '\0';
            }
        }

        HttpResponse res = router_dispatch(r, req_buf);

        char date_hdr[64] = {0};
        time_t now = time(NULL);
        struct tm* gmt = gmtime(&now);
        if (gmt) {
            strftime(date_hdr, sizeof(date_hdr), "%a, %d %b %Y %H:%M:%S GMT", gmt);
        }

        const char* body_data = res.body.data ? res.body.data : "";
        size_t body_len = strlen(body_data);
        const char* ctype = res.content_type.data ? res.content_type.data : "text/plain";
        const char* stext = http_status_text(res.status);

        char header_buf[1024];
        int hlen = snprintf(header_buf, sizeof(header_buf),
            "HTTP/1.1 %lld %s\r\n"
            "Server: Nyx-WebServer/0.53.0 (Zero-GC Region Core)\r\n"
            "Date: %s\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %zu\r\n"
            "Connection: close\r\n"
            "%s"
            "\r\n",
            (long long)res.status,
            stext,
            date_hdr,
            ctype,
            body_len,
            (r && r->enable_cors) ? "Access-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\nAccess-Control-Allow-Headers: Content-Type, Authorization, X-Requested-With\r\n" : ""
        );

        send(client_fd, header_buf, hlen, 0);
        if (body_len > 0) {
            send(client_fd, body_data, (int)body_len, 0);
        }

        /* Graceful TCP close to avoid RST packets on Windows Winsock */
#ifdef _WIN32
        shutdown(client_fd, SD_SEND);
#else
        shutdown(client_fd, SHUT_WR);
#endif
        char drain_buf[256];
        while (recv(client_fd, drain_buf, sizeof(drain_buf), 0) > 0) {}
        nyx_close_sock(client_fd);
        requests_served++;

        if (r && r->enable_logging) {
            char req_line[64] = {0};
            sscanf(req_buf, "%63[^\r\n]", req_line);
            printf("  [HTTP %d] %s -> %lld %s (%zu bytes)\n", requests_served, req_line, (long long)res.status, stext, body_len);
            fflush(stdout);
        }
    }

    nyx_close_sock(server_fd);
#ifdef _WIN32
    WSACleanup();
#endif
}

void serve(rt_string_t addr, void* r) {
    serve_with_options(addr, r, 0);
}
