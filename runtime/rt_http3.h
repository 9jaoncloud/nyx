#ifndef RT_HTTP3_H
#define RT_HTTP3_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "rt_string.h"
#include "rt_http.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── HTTP/3 & QUIC Constants (RFC 9000, RFC 9114, RFC 9204) ── */
#define HTTP3_ALPN_H3 "h3"
#define HTTP3_ALPN_H3_LEN 2

/* HTTP/3 Frame Types */
#define HTTP3_FRAME_DATA          0x00
#define HTTP3_FRAME_HEADERS       0x01
#define HTTP3_FRAME_CANCEL_PUSH   0x02
#define HTTP3_FRAME_SETTINGS      0x03
#define HTTP3_FRAME_PUSH_PROMISE  0x04
#define HTTP3_FRAME_GOAWAY        0x07
#define HTTP3_FRAME_MAX_PUSH_ID   0x0d

/* HTTP/3 Settings Identifiers */
#define HTTP3_SETTINGS_QPACK_MAX_TABLE_CAPACITY 0x01
#define HTTP3_SETTINGS_MAX_FIELD_SECTION_SIZE   0x06
#define HTTP3_SETTINGS_QPACK_BLOCKED_STREAMS    0x07

/* QUIC Packet Types */
#define QUIC_PACKET_INITIAL    0x00
#define QUIC_PACKET_0RTT       0x01
#define QUIC_PACKET_HANDSHAKE  0x02
#define QUIC_PACKET_RETRY      0x03
#define QUIC_PACKET_SHORT_1RTT 0x04

/* HTTP/3 Frame Header */
typedef struct {
    uint64_t type;    /* Varint frame type */
    uint64_t length;  /* Varint payload length */
} http3_frame_hdr_t;

/* QPACK Header Pair */
typedef struct {
    char name[128];
    char value[512];
} qpack_header_pair_t;

/* QUIC Connection ID */
#define QUIC_MAX_CID_LEN 20
typedef struct {
    uint8_t len;
    uint8_t id[QUIC_MAX_CID_LEN];
} quic_cid_t;

/* QUIC Packet Header (Parsed) */
typedef struct {
    bool is_long_header;
    uint8_t packet_type;
    uint32_t version;
    quic_cid_t dest_cid;
    quic_cid_t src_cid;
    uint64_t packet_number;
    size_t payload_offset;
    size_t payload_len;
} quic_packet_hdr_t;

/* ── QUIC Variable-Length Integer Codec (RFC 9000 §16) ── */
bool quic_varint_decode(const uint8_t* buf, size_t len, uint64_t* val_out, size_t* bytes_read);
size_t quic_varint_encode(uint64_t val, uint8_t* buf, size_t cap);
size_t quic_varint_len(uint64_t val);

/* ── QPACK Static Table Codec (RFC 9204) ── */
const char* qpack_get_static_name(uint32_t index);
const char* qpack_get_static_value(uint32_t index);
bool qpack_decode_field_section(const uint8_t* buf, size_t len,
                                qpack_header_pair_t* out_headers, size_t max_headers, size_t* num_headers_out);
size_t qpack_encode_header(uint8_t* out_buf, size_t out_cap, const char* name, const char* value);

/* ── HTTP/3 Frame Codec ── */
bool http3_parse_frame_hdr(const uint8_t* buf, size_t len, http3_frame_hdr_t* out_hdr, size_t* hdr_bytes_read);
size_t http3_serialize_frame(uint8_t* out_buf, size_t cap, uint64_t type, const uint8_t* payload, uint64_t payload_len);

/* ── QUIC Packet Header Codec ── */
bool quic_parse_packet_hdr(const uint8_t* buf, size_t len, quic_packet_hdr_t* out_hdr);

/* ── HTTP/3 Server Engine (over UDP Socket) ── */
void http3_serve(rt_string_t addr, Router* router);
void http3_serve_with_options(rt_string_t addr, Router* router, int max_requests);

#ifdef __cplusplus
}
#endif

#endif /* RT_HTTP3_H */
