/*
 * rt_http_app.h — Nyx Runtime Full-Featured Web Application Framework & Router
 */

#ifndef RT_HTTP_APP_H
#define RT_HTTP_APP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HTTP_METHOD_GET,
    HTTP_METHOD_POST,
    HTTP_METHOD_PUT,
    HTTP_METHOD_DELETE,
    HTTP_METHOD_WS
} HttpMethod;

typedef struct {
    HttpMethod method;
    char path[128];
    char query_params[256];
    char headers[512];
    char body[1024];
    char matched_param[64];
    char auth_user[64];
    bool is_authenticated;
} NyxHttpRequest;

typedef struct {
    int32_t status_code;
    char content_type[64];
    char body[2048];
    char headers[512];
} NyxHttpResponse;

typedef void (*NyxRouteHandler)(const NyxHttpRequest* req, NyxHttpResponse* res);
typedef bool (*NyxMiddlewareHandler)(NyxHttpRequest* req, NyxHttpResponse* res);

typedef struct {
    HttpMethod method;
    char path_pattern[128];
    NyxRouteHandler handler;
} NyxRoute;

typedef struct {
    NyxRoute routes[32];
    size_t route_count;
    NyxMiddlewareHandler middlewares[8];
    size_t middleware_count;
    char jwt_secret[64];
} NyxApp;

/* App & Router Lifecycle */
NyxApp* nyx_app_create(void);
void nyx_app_destroy(NyxApp* app);
void nyx_app_use_middleware(NyxApp* app, NyxMiddlewareHandler mw);
void nyx_app_set_jwt_secret(NyxApp* app, const char* secret);

/* Route Registration */
void nyx_app_get(NyxApp* app, const char* pattern, NyxRouteHandler handler);
void nyx_app_post(NyxApp* app, const char* pattern, NyxRouteHandler handler);
void nyx_app_ws(NyxApp* app, const char* pattern, NyxRouteHandler handler);

/* Request Dispatcher */
void nyx_app_dispatch(NyxApp* app, const char* method_str, const char* uri, const char* token, NyxHttpResponse* out_res);

/* Standard Middleware Helpers */
bool nyx_mw_cors(NyxHttpRequest* req, NyxHttpResponse* res);
bool nyx_mw_jwt_auth(NyxHttpRequest* req, NyxHttpResponse* res);

/* JWT Helper */
bool nyx_jwt_sign(const char* username, const char* secret, char* out_jwt, size_t buf_len);
bool nyx_jwt_verify(const char* jwt, const char* secret, char* out_username, size_t buf_len);

#ifdef __cplusplus
}
#endif

#endif /* RT_HTTP_APP_H */
