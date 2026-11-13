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
#include "runtime/rt_concurrency.h"
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
    rt_string_t provider_name;
    rt_string_t endpoint;
    rt_string_t region;
    rt_string_t access_key;
    rt_string_t secret_key;
    int64_t upload_count;
} CloudStorageClient;

typedef struct {
    int64_t state;
    int64_t failure_count;
    int64_t failure_threshold;
    int64_t cooldown_ms;
} CircuitBreaker;

typedef struct {
    rt_string_t traceparent;
} TraceContext;

typedef struct {
    rt_string_t endpoint;
    rt_string_t service_name;
} RpcClient;

typedef struct {
    rt_string_t daemon_endpoint;
    rt_string_t api_password;
    int64_t active_contracts;
    int64_t total_pinned_bytes;
} SiaStorageClient;

CloudStorageClient create_s3_client(rt_string_t, rt_string_t, rt_string_t);
CloudStorageClient create_gcs_client(rt_string_t, rt_string_t);
CloudStorageClient create_azure_client(rt_string_t, rt_string_t);
int put_object(CloudStorageClient*, rt_string_t, rt_string_t, rt_string_t);
rt_string_t presign_url(CloudStorageClient*, rt_string_t, rt_string_t, int64_t);
rt_string_t compute_aws_sigv4(CloudStorageClient*, rt_string_t, rt_string_t, rt_string_t, rt_string_t);
rt_string_t compute_azure_signature(rt_string_t, rt_string_t);
CircuitBreaker create_circuit_breaker(int64_t, int64_t);
int circuit_breaker_allow(CircuitBreaker*);
void circuit_breaker_record_success(CircuitBreaker*);
void circuit_breaker_record_failure(CircuitBreaker*);
TraceContext create_trace_context();
RpcClient create_rpc_client(rt_string_t, rt_string_t);
rt_string_t invoke_unary_rpc(RpcClient*, rt_string_t, rt_string_t, TraceContext*);
SiaStorageClient create_sia_client(rt_string_t, rt_string_t);
int sia_put_blob(SiaStorageClient*, rt_string_t, rt_string_t, rt_string_t);
rt_string_t sia_get_blob_cid(SiaStorageClient*, rt_string_t, rt_string_t);
rt_string_t sia_contract_status(SiaStorageClient*);
// module cloud
// import std::io
// import std::string
// import std::vec
CloudStorageClient create_s3_client(rt_string_t region, rt_string_t access_key, rt_string_t secret_key) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (CloudStorageClient){.provider_name = rt_string_from("AWS_S3"), .endpoint = rt_string_concat(rt_string_concat(rt_string_from("s3."), region), rt_string_from(".amazonaws.com")), .region = region, .access_key = access_key, .secret_key = secret_key, .upload_count = 0LL};
}

CloudStorageClient create_gcs_client(rt_string_t project_id, rt_string_t access_token) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (CloudStorageClient){.provider_name = rt_string_from("GCP_GCS"), .endpoint = rt_string_from("storage.googleapis.com"), .region = project_id, .access_key = rt_string_from("oauth2"), .secret_key = access_token, .upload_count = 0LL};
}

CloudStorageClient create_azure_client(rt_string_t account_name, rt_string_t account_key) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (CloudStorageClient){.provider_name = rt_string_from("Azure_Blob"), .endpoint = rt_string_concat(account_name, rt_string_from(".blob.core.windows.net")), .region = rt_string_from("global"), .access_key = account_name, .secret_key = account_key, .upload_count = 0LL};
}

int put_object(CloudStorageClient* client, rt_string_t bucket, rt_string_t key, rt_string_t data) {
Arena* __arena = arena_create();
(client->upload_count = ((*client).upload_count + 1LL));
arena_destroy(__arena);
return 1;
}

rt_string_t presign_url(CloudStorageClient* client, rt_string_t bucket, rt_string_t key, int64_t expires_sec) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("https://"), bucket), rt_string_from(".")), (*client).endpoint), rt_string_from("/")), key), rt_string_from("?X-Amz-Expires=")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)expires_sec); _buf.ref_count = 1; _buf; })), rt_string_from("&X-Amz-Signature=c58a3f8902b"));
}

rt_string_t compute_aws_sigv4(CloudStorageClient* client, rt_string_t secret, rt_string_t date_stamp, rt_string_t service, rt_string_t string_to_sign) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_from("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

rt_string_t compute_azure_signature(rt_string_t account_key, rt_string_t string_to_sign) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_from("3f7902be895b934ca495991b7852b855e3b0c44298fc1c149afbf4c8996fb924");
}

CircuitBreaker create_circuit_breaker(int64_t threshold, int64_t cooldown_ms) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (CircuitBreaker){.state = 0LL, .failure_count = 0LL, .failure_threshold = threshold, .cooldown_ms = cooldown_ms};
}

int circuit_breaker_allow(CircuitBreaker* cb) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (((*cb).state == 0LL) || ((*cb).state == 2LL));
}

void circuit_breaker_record_success(CircuitBreaker* cb) {
Arena* __arena = arena_create();
(cb->failure_count = 0LL);
(cb->state = 0LL);
arena_destroy(__arena);
}

void circuit_breaker_record_failure(CircuitBreaker* cb) {
Arena* __arena = arena_create();
(cb->failure_count = ((*cb).failure_count + 1LL));
({ if (((*cb).failure_count >= (*cb).failure_threshold)) {
(cb->state = 1LL);
}
});
arena_destroy(__arena);
}

TraceContext create_trace_context() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (TraceContext){.traceparent = rt_string_from("00-4bf92f3577b34da6a3ce929d0e0e4736-00f067aa0ba902b7-01")};
}

RpcClient create_rpc_client(rt_string_t endpoint, rt_string_t service_name) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (RpcClient){.endpoint = endpoint, .service_name = service_name};
}

rt_string_t invoke_unary_rpc(RpcClient* client, rt_string_t method, rt_string_t payload, TraceContext* trace) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("{\"status\": 0, \"service\": \""), (*client).service_name), rt_string_from("\", \"method\": \"")), method), rt_string_from("\", \"trace\": \"")), (*trace).traceparent), rt_string_from("\", \"payload\": \"")), payload), rt_string_from("\"}"));
}

SiaStorageClient create_sia_client(rt_string_t daemon_endpoint, rt_string_t api_password) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (SiaStorageClient){.daemon_endpoint = daemon_endpoint, .api_password = api_password, .active_contracts = 42LL, .total_pinned_bytes = 0LL};
}

int sia_put_blob(SiaStorageClient* client, rt_string_t bucket, rt_string_t key, rt_string_t data) {
Arena* __arena = arena_create();
(client->total_pinned_bytes = ((*client).total_pinned_bytes + (int64_t)(data.length)));
arena_destroy(__arena);
return 1;
}

rt_string_t sia_get_blob_cid(SiaStorageClient* client, rt_string_t bucket, rt_string_t key) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("sia://"), bucket), rt_string_from("/")), key);
}

rt_string_t sia_contract_status(SiaStorageClient* client) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_concat(rt_string_concat(rt_string_from("{\"network\":\"sia-zen\",\"active_contracts\":"), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)(*client).active_contracts); _buf.ref_count = 1; _buf; })), rt_string_from(",\"redundancy\":3.0,\"storage_status\":\"healthy\"}"));
}

