#ifndef RT_HTTP2_H
#define RT_HTTP2_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "rt_string.h"
#include "rt_http.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── HTTP/2 Constants (RFC 9113) ── */
#define HTTP2_PREFACE "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n"
#define HTTP2_PREFACE_LEN 24
#define HTTP2_FRAME_HDR_LEN 9
#define HTTP2_MAX_FRAME_SIZE 16384
#define HTTP2_DEFAULT_WINDOW_SIZE 65535

/* Frame Types */
#define HTTP2_FRAME_DATA          0x00
#define HTTP2_FRAME_HEADERS       0x01
#define HTTP2_FRAME_PRIORITY      0x02
#define HTTP2_FRAME_RST_STREAM    0x03
#define HTTP2_FRAME_SETTINGS      0x04
#define HTTP2_FRAME_PUSH_PROMISE  0x05
#define HTTP2_FRAME_PING          0x06
#define HTTP2_FRAME_GOAWAY        0x07
#define HTTP2_FRAME_WINDOW_UPDATE 0x08
#define HTTP2_FRAME_CONTINUATION  0x09

/* Frame Flags */
#define HTTP2_FLAG_NONE           0x00
#define HTTP2_FLAG_END_STREAM     0x01
#define HTTP2_FLAG_ACK            0x01
#define HTTP2_FLAG_END_HEADERS    0x04
#define HTTP2_FLAG_PADDED         0x08
#define HTTP2_FLAG_PRIORITY       0x20

/* Error Codes */
#define HTTP2_ERR_NO_ERROR            0x00
#define HTTP2_ERR_PROTOCOL_ERROR      0x01
#define HTTP2_ERR_INTERNAL_ERROR      0x02
#define HTTP2_ERR_FLOW_CONTROL_ERROR  0x03
#define HTTP2_ERR_SETTINGS_TIMEOUT    0x04
#define HTTP2_ERR_STREAM_CLOSED       0x05
#define HTTP2_ERR_FRAME_SIZE_ERROR    0x06
#define HTTP2_ERR_REFUSED_STREAM      0x07
#define HTTP2_ERR_CANCEL              0x08
#define HTTP2_ERR_COMPRESSION_ERROR   0x09
#define HTTP2_ERR_CONNECT_ERROR       0x0a

/* Stream States */
typedef enum {
    HTTP2_STREAM_IDLE = 0,
    HTTP2_STREAM_RESERVED,
    HTTP2_STREAM_OPEN,
    HTTP2_STREAM_HALF_CLOSED_LOCAL,
    HTTP2_STREAM_HALF_CLOSED_REMOTE,
    HTTP2_STREAM_CLOSED
} http2_stream_state_t;

/* HTTP/2 9-Byte Frame Header */
typedef struct {
    uint32_t length;     /* 24-bit payload length */
    uint8_t  type;       /* 8-bit frame type */
    uint8_t  flags;      /* 8-bit flags */
    uint32_t stream_id;  /* 31-bit stream identifier */
} http2_frame_hdr_t;

/* Header Key/Value Pair */
typedef struct {
    char name[128];
    char value[512];
} http2_header_pair_t;

/* HPACK Dynamic Table Entry */
typedef struct {
    char name[128];
    char value[512];
} hpack_entry_t;

#define HPACK_DYNAMIC_TABLE_MAX_ENTRIES 64
typedef struct {
    hpack_entry_t entries[HPACK_DYNAMIC_TABLE_MAX_ENTRIES];
    size_t count;
    size_t head;
    size_t size_bytes;
    size_t max_size_bytes;
} hpack_dynamic_table_t;

/* HTTP/2 Stream Context */
typedef struct {
    uint32_t stream_id;
    http2_stream_state_t state;
    int32_t inbound_window;
    int32_t outbound_window;
    char method[16];
    char path[256];
    char scheme[16];
    char authority[128];
    char body_buf[4096];
    size_t body_len;
} http2_stream_t;

#define HTTP2_MAX_CONCURRENT_STREAMS 32

/* HTTP/2 Connection Context */
typedef struct {
    int32_t client_fd;
    hpack_dynamic_table_t hpack_in;
    hpack_dynamic_table_t hpack_out;
    http2_stream_t streams[HTTP2_MAX_CONCURRENT_STREAMS];
    size_t num_streams;
    int32_t conn_inbound_window;
    int32_t conn_outbound_window;
    bool settings_acknowledged;
} http2_conn_t;

/* ── HPACK Codec API (RFC 7541) ── */
void hpack_dynamic_table_init(hpack_dynamic_table_t* tbl, size_t max_bytes);
bool hpack_decode(const uint8_t* buf, size_t len, hpack_dynamic_table_t* dyn_tbl,
                  http2_header_pair_t* out_headers, size_t max_headers, size_t* num_headers_out);
size_t hpack_encode_header(uint8_t* out_buf, size_t out_cap, const char* name, const char* value);
const char* hpack_get_static_name(uint32_t index);
const char* hpack_get_static_value(uint32_t index);

/* ── Binary Frame Codec API ── */
bool http2_parse_frame_hdr(const uint8_t* buf, size_t len, http2_frame_hdr_t* out_hdr);
size_t http2_serialize_frame_hdr(uint8_t* out_buf, size_t cap, uint32_t length, uint8_t type, uint8_t flags, uint32_t stream_id);

/* ── HTTP/2 Server Engine ── */
void http2_serve(rt_string_t addr, Router* router);
void http2_serve_with_options(rt_string_t addr, Router* router, int max_requests);

#ifdef __cplusplus
}
#endif

#endif /* RT_HTTP2_H */
