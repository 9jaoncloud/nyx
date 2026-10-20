/*
 * rt_http_app.c — Nyx Runtime Web Application Framework Implementation
 */

#include "rt_http_app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NyxApp* nyx_app_create(void) {
    NyxApp* app = (NyxApp*)malloc(sizeof(NyxApp));
    if (!app) return NULL;
    app->route_count = 0;
    app->middleware_count = 0;
    strncpy(app->jwt_secret, "nyx_secret_jwt_key_2026", sizeof(app->jwt_secret) - 1);
    return app;
}

void nyx_app_destroy(NyxApp* app) {
    if (app) free(app);
}

void nyx_app_use_middleware(NyxApp* app, NyxMiddlewareHandler mw) {
    if (!app || !mw || app->middleware_count >= 8) return;
    app->middlewares[app->middleware_count++] = mw;
}

void nyx_app_set_jwt_secret(NyxApp* app, const char* secret) {
    if (app && secret) strncpy(app->jwt_secret, secret, sizeof(app->jwt_secret) - 1);
}

static void register_route(NyxApp* app, HttpMethod method, const char* pattern, NyxRouteHandler handler) {
    if (!app || !pattern || !handler || app->route_count >= 32) return;
    NyxRoute* r = &app->routes[app->route_count++];
    r->method = method;
    strncpy(r->path_pattern, pattern, sizeof(r->path_pattern) - 1);
    r->handler = handler;
}

void nyx_app_get(NyxApp* app, const char* pattern, NyxRouteHandler handler) {
    register_route(app, HTTP_METHOD_GET, pattern, handler);
}

void nyx_app_post(NyxApp* app, const char* pattern, NyxRouteHandler handler) {
    register_route(app, HTTP_METHOD_POST, pattern, handler);
}

void nyx_app_ws(NyxApp* app, const char* pattern, NyxRouteHandler handler) {
    register_route(app, HTTP_METHOD_WS, pattern, handler);
}

/* ── JWT Simulation (HMAC-SHA256 Token) ─────────────────────────────────── */
bool nyx_jwt_sign(const char* username, const char* secret, char* out_jwt, size_t buf_len) {
    if (!username || !secret || !out_jwt || buf_len < 64) return false;
    snprintf(out_jwt, buf_len, "eyJhbGciOiJIUzI1NiJ9.%s.sig_%08x", username, (unsigned int)(strlen(username) * 7919 + strlen(secret) * 104729));
    return true;
}

bool nyx_jwt_verify(const char* jwt, const char* secret, char* out_username, size_t buf_len) {
    if (!jwt || !secret || !out_username || buf_len == 0) return false;
    char payload[64];
    char sig[64];
    if (sscanf(jwt, "eyJhbGciOiJIUzI1NiJ9.%[^.].%s", payload, sig) == 2) {
        char expected_sig[64];
        snprintf(expected_sig, sizeof(expected_sig), "sig_%08x", (unsigned int)(strlen(payload) * 7919 + strlen(secret) * 104729));
        if (strcmp(sig, expected_sig) == 0) {
            strncpy(out_username, payload, buf_len - 1);
            out_username[buf_len - 1] = '\0';
            return true;
        }
    }
    return false;
}

/* ── Standard Middlewares ────────────────────────────────────────────────── */
bool nyx_mw_cors(NyxHttpRequest* req, NyxHttpResponse* res) {
    (void)req;
    strncat(res->headers, "Access-Control-Allow-Origin: *\n", sizeof(res->headers) - strlen(res->headers) - 1);
    strncat(res->headers, "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\n", sizeof(res->headers) - strlen(res->headers) - 1);
    return true; /* continue pipeline */
}

bool nyx_mw_jwt_auth(NyxHttpRequest* req, NyxHttpResponse* res) {
    if (req->is_authenticated) return true;
    res->status_code = 401;
    strncpy(res->content_type, "application/json", sizeof(res->content_type) - 1);
    strncpy(res->body, "{\"error\": \"Unauthorized: Missing or Invalid JWT Bearer Token\"}", sizeof(res->body) - 1);
    return false; /* abort pipeline */
}

/* ── Route Matcher & Parameter Extraction ────────────────────────────────── */
static bool match_route_pattern(const char* pattern, const char* uri, char* out_param, size_t param_len) {
    /* Pattern: /api/v1/stocks/:ticker */
    const char* p_colon = strstr(pattern, ":");
    if (!p_colon) return (strcmp(pattern, uri) == 0);

    size_t prefix_len = p_colon - pattern;
    if (strncmp(pattern, uri, prefix_len) != 0) return false;

    const char* param_val = uri + prefix_len;
    if (out_param && param_len > 0) {
        strncpy(out_param, param_val, param_len - 1);
        out_param[param_len - 1] = '\0';
    }
    return true;
}

void nyx_app_dispatch(NyxApp* app, const char* method_str, const char* uri, const char* token, NyxHttpResponse* out_res) {
    if (!app || !method_str || !uri || !out_res) return;
    memset(out_res, 0, sizeof(NyxHttpResponse));
    out_res->status_code = 404;
    strncpy(out_res->content_type, "application/json", sizeof(out_res->content_type) - 1);
    strncpy(out_res->body, "{\"error\": \"Route Not Found\"}", sizeof(out_res->body) - 1);

    NyxHttpRequest req;
    memset(&req, 0, sizeof(req));
    strncpy(req.path, uri, sizeof(req.path) - 1);

    if (strcmp(method_str, "GET") == 0) req.method = HTTP_METHOD_GET;
    else if (strcmp(method_str, "POST") == 0) req.method = HTTP_METHOD_POST;
    else if (strcmp(method_str, "WS") == 0) req.method = HTTP_METHOD_WS;

    if (token && nyx_jwt_verify(token, app->jwt_secret, req.auth_user, sizeof(req.auth_user))) {
        req.is_authenticated = true;
    }

    /* Execute Middleware Pipeline */
    for (size_t m = 0; m < app->middleware_count; m++) {
        if (!app->middlewares[m](&req, out_res)) {
            return; /* Middleware stopped execution */
        }
    }

    /* Match Routes */
    for (size_t r = 0; r < app->route_count; r++) {
        NyxRoute* route = &app->routes[r];
        if (route->method == req.method) {
            char param_buf[64] = {0};
            if (match_route_pattern(route->path_pattern, uri, param_buf, sizeof(param_buf))) {
                strncpy(req.matched_param, param_buf, sizeof(req.matched_param) - 1);
                out_res->status_code = 200;
                route->handler(&req, out_res);
                return;
            }
        }
    }
}
