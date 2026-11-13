#ifndef RT_WS_H
#define RT_WS_H
/* Nyx runtime — WebSocket (RFC 6455) helper primitives.
 * Header-only (static inline) so the C-emitting bootstrap picks them up
 * without rebuilding libnyxrt.a. Provides the SHA-1 + Base64 the RFC 6455
 * handshake needs (the runtime only ships SHA-256/512/MD5 in rt_crypto). */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "rt_crypto.h"

static inline void nyx_sha1(const uint8_t* data, size_t len, uint8_t out[20]) {
    uint32_t h0 = 0x67452301u, h1 = 0xEFCDAB89u, h2 = 0x98BADCFEu,
             h3 = 0x10325476u, h4 = 0xC3D2E1F0u;
    uint64_t ml = (uint64_t)len * 8u;
    size_t total = len + 1;
    while (total % 64u != 56u) total++;
    total += 8;
    uint8_t* msg = (uint8_t*)calloc(total, 1);
    if (!msg) return;
    if (len) memcpy(msg, data, len);
    msg[len] = 0x80;
    for (int i = 0; i < 8; i++) msg[total - 1 - (size_t)i] = (uint8_t)(ml >> (8 * i));
    for (size_t off = 0; off < total; off += 64) {
        uint32_t w[80];
        for (int i = 0; i < 16; i++) {
            w[i] = ((uint32_t)msg[off + i * 4] << 24) |
                   ((uint32_t)msg[off + i * 4 + 1] << 16) |
                   ((uint32_t)msg[off + i * 4 + 2] << 8) |
                   ((uint32_t)msg[off + i * 4 + 3]);
        }
        for (int i = 16; i < 80; i++) {
            uint32_t v = w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16];
            w[i] = (v << 1) | (v >> 31);
        }
        uint32_t a = h0, b = h1, c = h2, d = h3, e = h4;
        for (int i = 0; i < 80; i++) {
            uint32_t f, k;
            if (i < 20)      { f = (b & c) | ((~b) & d);          k = 0x5A827999u; }
            else if (i < 40) { f = b ^ c ^ d;                     k = 0x6ED9EBA1u; }
            else if (i < 60) { f = (b & c) | (b & d) | (c & d);   k = 0x8F1BBCDCu; }
            else             { f = b ^ c ^ d;                     k = 0xCA62C1D6u; }
            uint32_t tmp = ((a << 5) | (a >> 27)) + f + e + k + w[i];
            e = d; d = c; c = (b << 30) | (b >> 2); b = a; a = tmp;
        }
        h0 += a; h1 += b; h2 += c; h3 += d; h4 += e;
    }
    free(msg);
    uint32_t hs[5] = { h0, h1, h2, h3, h4 };
    for (int i = 0; i < 5; i++) {
        out[i * 4 + 0] = (uint8_t)(hs[i] >> 24);
        out[i * 4 + 1] = (uint8_t)(hs[i] >> 16);
        out[i * 4 + 2] = (uint8_t)(hs[i] >> 8);
        out[i * 4 + 3] = (uint8_t)(hs[i]);
    }
}

static inline size_t nyx_base64_encode(const uint8_t* in, size_t len, char* out) {
    static const char T[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t o = 0, i = 0;
    for (; i + 3 <= len; i += 3) {
        uint32_t n = ((uint32_t)in[i] << 16) | ((uint32_t)in[i + 1] << 8) | (uint32_t)in[i + 2];
        out[o++] = T[(n >> 18) & 63]; out[o++] = T[(n >> 12) & 63];
        out[o++] = T[(n >> 6) & 63];  out[o++] = T[n & 63];
    }
    if (len - i == 1) {
        uint32_t n = (uint32_t)in[i] << 16;
        out[o++] = T[(n >> 18) & 63]; out[o++] = T[(n >> 12) & 63];
        out[o++] = '='; out[o++] = '=';
    } else if (len - i == 2) {
        uint32_t n = ((uint32_t)in[i] << 16) | ((uint32_t)in[i + 1] << 8);
        out[o++] = T[(n >> 18) & 63]; out[o++] = T[(n >> 12) & 63];
        out[o++] = T[(n >> 6) & 63];  out[o++] = '=';
    }
    out[o] = '\0';
    return o;
}

#define NYX_WS_GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

/* Generate a 16-byte random nonce, base64-encoded (24 chars) into `out`. */
static inline void nyx_ws_client_key(char* out) {
    uint8_t rnd[16];
    nyx_random_bytes(rnd, 16);
    nyx_base64_encode(rnd, 16, out);
}

/* RFC 6455 Sec-WebSocket-Accept = base64(SHA1(key + GUID)). `out` needs >= 29 bytes. */
static inline void nyx_ws_accept_key(const char* client_key, char* out) {
    char buf[512];
    size_t kl = client_key ? strlen(client_key) : 0;
    if (kl > 256) kl = 256;
    if (kl && client_key) memcpy(buf, client_key, kl);
    size_t gl = strlen(NYX_WS_GUID);
    memcpy(buf + kl, NYX_WS_GUID, gl);
    uint8_t dig[20];
    nyx_sha1((const uint8_t*)buf, kl + gl, dig);
    nyx_base64_encode(dig, 20, out);
}

#endif
