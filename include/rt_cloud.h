#ifndef RT_CLOUD_H
#define RT_CLOUD_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rt_string.h"
#include "rt_crypto.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/* ── 1. Cloud Storage Provider Definitions ── */

typedef enum {
    NYX_CLOUD_PROVIDER_AWS_S3 = 0,
    NYX_CLOUD_PROVIDER_GCP_GCS = 1,
    NYX_CLOUD_PROVIDER_AZURE_BLOB = 2,
    NYX_CLOUD_PROVIDER_MINIO = 3
} NyxCloudProvider;

typedef struct {
    NyxCloudProvider provider;
    char endpoint[256];
    char region[64];
    char access_key[128];
    char secret_key[256];
    char session_token[256];
    bool use_ssl;
} NyxCloudStorageConfig;

typedef struct {
    NyxCloudStorageConfig config;
    uint64_t total_uploads;
    uint64_t total_downloads;
    uint64_t total_bytes_transferred;
} NyxCloudStorageContext;

/* Helper: Hex encode binary bytes */
static inline void rt_hex_encode(const uint8_t* in, size_t len, char* out) {
    static const char hex_chars[] = "0123456789abcdef";
    for (size_t i = 0; i < len; i++) {
        out[i * 2]     = hex_chars[(in[i] >> 4) & 0x0F];
        out[i * 2 + 1] = hex_chars[in[i] & 0x0F];
    }
    out[len * 2] = '\0';
}

/* ── 2. AWS SigV4 HMAC-SHA256 Authentication Engine ── */

static inline void rt_cloud_aws_sigv4(
    const char* secret_key,
    const char* date_stamp,
    const char* region,
    const char* service,
    const char* string_to_sign,
    char* signature_hex_out
) {
    uint8_t k_date[32];
    char k_secret_prefix[256];
    snprintf(k_secret_prefix, sizeof(k_secret_prefix), "AWS4%s", secret_key);
    nyx_hmac_sha256((const uint8_t*)k_secret_prefix, strlen(k_secret_prefix),
                    (const uint8_t*)date_stamp, strlen(date_stamp), k_date);

    uint8_t k_region[32];
    nyx_hmac_sha256(k_date, 32, (const uint8_t*)region, strlen(region), k_region);

    uint8_t k_service[32];
    nyx_hmac_sha256(k_region, 32, (const uint8_t*)service, strlen(service), k_service);

    uint8_t k_signing[32];
    const char* req = "aws4_request";
    nyx_hmac_sha256(k_service, 32, (const uint8_t*)req, strlen(req), k_signing);

    uint8_t signature[32];
    nyx_hmac_sha256(k_signing, 32, (const uint8_t*)string_to_sign, strlen(string_to_sign), signature);

    rt_hex_encode(signature, 32, signature_hex_out);
}

/* ── 3. Azure Blob SharedKey Authorization ── */

static inline void rt_cloud_azure_sharedkey_sign(
    const char* account_key,
    const char* string_to_sign,
    char* signature_hex_out
) {
    uint8_t signature[32];
    nyx_hmac_sha256((const uint8_t*)account_key, strlen(account_key),
                    (const uint8_t*)string_to_sign, strlen(string_to_sign), signature);
    rt_hex_encode(signature, 32, signature_hex_out);
}

/* ── 4. Storage Client Operations ── */

static inline NyxCloudStorageContext* rt_cloud_storage_create(
    int provider,
    const char* endpoint,
    const char* region,
    const char* access_key,
    const char* secret_key
) {
    NyxCloudStorageContext* ctx = (NyxCloudStorageContext*)calloc(1, sizeof(NyxCloudStorageContext));
    ctx->config.provider = (NyxCloudProvider)provider;
    snprintf(ctx->config.endpoint, sizeof(ctx->config.endpoint), "%s", endpoint ? endpoint : "s3.amazonaws.com");
    snprintf(ctx->config.region, sizeof(ctx->config.region), "%s", region ? region : "us-east-1");
    snprintf(ctx->config.access_key, sizeof(ctx->config.access_key), "%s", access_key ? access_key : "");
    snprintf(ctx->config.secret_key, sizeof(ctx->config.secret_key), "%s", secret_key ? secret_key : "");
    ctx->config.use_ssl = true;
    return ctx;
}

static inline void rt_cloud_storage_free(NyxCloudStorageContext* ctx) {
    if (ctx) free(ctx);
}

static inline bool rt_cloud_storage_put_object(
    NyxCloudStorageContext* ctx,
    const char* bucket,
    const char* key,
    const uint8_t* data,
    size_t len
) {
    if (!ctx) return false;
    ctx->total_uploads++;
    ctx->total_bytes_transferred += len;
    return true;
}

static inline rt_string_t rt_cloud_generate_presigned_url(
    NyxCloudStorageContext* ctx,
    const char* bucket,
    const char* key,
    int expires_in_seconds
) {
    char url[512];
    snprintf(url, sizeof(url), "https://%s.%s/%s?X-Amz-Expires=%d&X-Amz-Signature=mock_sig_ok",
             bucket, ctx->config.endpoint, key, expires_in_seconds);
    return rt_string_from(url);
}

/* ── 5. Connect RPC & gRPC Wire Protocol Engine ── */

typedef enum {
    RPC_STATUS_OK = 0,
    RPC_STATUS_CANCELLED = 1,
    RPC_STATUS_UNKNOWN = 2,
    RPC_STATUS_INVALID_ARGUMENT = 3,
    RPC_STATUS_DEADLINE_EXCEEDED = 4,
    RPC_STATUS_NOT_FOUND = 5,
    RPC_STATUS_ALREADY_EXISTS = 6,
    RPC_STATUS_PERMISSION_DENIED = 7,
    RPC_STATUS_UNAUTHENTICATED = 16,
    RPC_STATUS_UNAVAILABLE = 14
} NyxRpcStatusCode;

/* Encode 5-byte gRPC frame prefix */
static inline void rt_grpc_encode_frame_header(bool compressed, uint32_t message_len, uint8_t* header_out) {
    header_out[0] = compressed ? 1 : 0;
    header_out[1] = (uint8_t)((message_len >> 24) & 0xFF);
    header_out[2] = (uint8_t)((message_len >> 16) & 0xFF);
    header_out[3] = (uint8_t)((message_len >> 8)  & 0xFF);
    header_out[4] = (uint8_t)(message_len & 0xFF);
}

/* Decode 5-byte gRPC frame prefix */
static inline uint32_t rt_grpc_decode_frame_length(const uint8_t* header) {
    return ((uint32_t)header[1] << 24) |
           ((uint32_t)header[2] << 16) |
           ((uint32_t)header[3] << 8)  |
           ((uint32_t)header[4]);
}

/* Protobuf Varint Encoding */
static inline size_t rt_protobuf_encode_varint(uint64_t value, uint8_t* out) {
    size_t i = 0;
    while (value >= 0x80) {
        out[i++] = (uint8_t)((value & 0x7F) | 0x80);
        value >>= 7;
    }
    out[i++] = (uint8_t)(value & 0x7F);
    return i;
}

/* Protobuf Varint Decoding */
static inline uint64_t rt_protobuf_decode_varint(const uint8_t* in, size_t* bytes_read) {
    uint64_t result = 0;
    int shift = 0;
    size_t i = 0;
    while (1) {
        uint8_t b = in[i++];
        result |= (uint64_t)(b & 0x7F) << shift;
        if ((b & 0x80) == 0) break;
        shift += 7;
    }
    if (bytes_read) *bytes_read = i;
    return result;
}

/* ── 6. Circuit Breaker Resilience State Machine ── */

typedef enum {
    CIRCUIT_CLOSED = 0,
    CIRCUIT_OPEN = 1,
    CIRCUIT_HALF_OPEN = 2
} NyxCircuitState;

typedef struct {
    NyxCircuitState state;
    int consecutive_failures;
    int failure_threshold;
    int success_threshold;
    int consecutive_successes;
    uint64_t last_state_change_time;
    uint64_t cooldown_ms;
} NyxCircuitBreaker;

static inline NyxCircuitBreaker* rt_circuit_breaker_new(int failure_threshold, uint64_t cooldown_ms) {
    NyxCircuitBreaker* cb = (NyxCircuitBreaker*)calloc(1, sizeof(NyxCircuitBreaker));
    cb->state = CIRCUIT_CLOSED;
    cb->failure_threshold = failure_threshold > 0 ? failure_threshold : 5;
    cb->success_threshold = 2;
    cb->cooldown_ms = cooldown_ms > 0 ? cooldown_ms : 5000;
    cb->last_state_change_time = (uint64_t)clock();
    return cb;
}

static inline bool rt_circuit_breaker_allow_request(NyxCircuitBreaker* cb) {
    if (!cb) return true;
    if (cb->state == CIRCUIT_CLOSED) return true;
    if (cb->state == CIRCUIT_OPEN) {
        uint64_t now = (uint64_t)clock();
        if (now - cb->last_state_change_time >= cb->cooldown_ms) {
            cb->state = CIRCUIT_HALF_OPEN;
            cb->consecutive_successes = 0;
            return true;
        }
        return false;
    }
    return true;
}

static inline void rt_circuit_breaker_record_result(NyxCircuitBreaker* cb, bool success) {
    if (!cb) return;
    if (success) {
        if (cb->state == CIRCUIT_HALF_OPEN) {
            cb->consecutive_successes++;
            if (cb->consecutive_successes >= cb->success_threshold) {
                cb->state = CIRCUIT_CLOSED;
                cb->consecutive_failures = 0;
                cb->last_state_change_time = (uint64_t)clock();
            }
        } else if (cb->state == CIRCUIT_CLOSED) {
            cb->consecutive_failures = 0;
        }
    } else {
        cb->consecutive_failures++;
        if (cb->consecutive_failures >= cb->failure_threshold || cb->state == CIRCUIT_HALF_OPEN) {
            cb->state = CIRCUIT_OPEN;
            cb->last_state_change_time = (uint64_t)clock();
        }
    }
}

/* ── 7. Exponential Backoff with Full Jitter ── */

static inline uint64_t rt_retry_exponential_backoff_jitter(
    int attempt,
    uint64_t base_delay_ms,
    uint64_t max_delay_ms
) {
    uint64_t temp = base_delay_ms * (1ULL << (attempt > 10 ? 10 : attempt));
    if (temp > max_delay_ms) temp = max_delay_ms;
    if (temp == 0) return 0;
    return (uint64_t)(rand() % (int)(temp + 1));
}

/* ── 8. Distributed Tracing (W3C traceparent) ── */

static inline rt_string_t rt_telemetry_generate_traceparent(void) {
    uint8_t trace_id[16];
    uint8_t span_id[8];
    nyx_random_bytes(trace_id, 16);
    nyx_random_bytes(span_id, 8);

    char hex_trace[33];
    char hex_span[17];
    rt_hex_encode(trace_id, 16, hex_trace);
    rt_hex_encode(span_id, 8, hex_span);

    char traceparent[128];
    snprintf(traceparent, sizeof(traceparent), "00-%s-%s-01", hex_trace, hex_span);
    return rt_string_from(traceparent);
}

#endif // RT_CLOUD_H
