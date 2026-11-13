#ifndef RT_NET_H
#define RT_NET_H
#include <stdint.h>
#include <stddef.h>
#include "rt_string.h"
#include "rt_ws.h"   /* SHA-1 / Base64 / RFC 6455 handshake helpers */

#ifdef __cplusplus
extern "C" {
#endif

/* ── Socket Init ── */
void nyx_socket_init(void);

/* ── Raw Socket Functions (std.net) ── */
int32_t nyx_socket(int32_t domain, int32_t type, int32_t protocol);
int32_t nyx_bind(int32_t sock, const char* addr, int32_t port);
int32_t nyx_listen(int32_t sock, int32_t backlog);
int32_t nyx_accept(int32_t sock);
int32_t nyx_connect(const char* host, int32_t port);
int64_t nyx_send(int32_t sock, const char* data, uint64_t len);
int64_t nyx_recv(int32_t sock, char* buf, uint64_t len);
void nyx_close(int32_t sock);
void nyx_set_nonblocking(int32_t sock, int32_t enable);
void nyx_set_timeout(int32_t sock, int32_t seconds);
/* Writes the peer IP into `buf` (NUL-terminated) and returns the peer port,
   or -1 on error. Gives std.net a real `peer_addr` instead of a stub. */
int32_t nyx_peer_addr(int32_t sock, char* buf, int32_t buflen);

/* ── WebSocket (RFC 6455) client helpers (framing done in C, not Nyx) ── */
int32_t nyx_ws_handshake(int32_t sock, const char* host, int32_t hlen, int32_t port,
                         const char* path, int32_t plen);
int64_t nyx_ws_send(int32_t sock, int32_t opcode, const char* data, int64_t len);
int64_t nyx_ws_recv(int32_t sock, char* out, int32_t max, int32_t* opcode, int32_t* fin);

/* ── UDP Functions ── */
int32_t nyx_udp_socket(int32_t domain);
int64_t nyx_udp_sendto(int32_t sock, const char* data, uint64_t len, const char* addr, int32_t port);
int64_t nyx_udp_recvfrom(int32_t sock, char* buf, uint64_t len, char* addr, int32_t* port);

/* ── TCP Higher-Level Helpers (std.http) ── */
int32_t nyx_tcp_connect(const char* host, int32_t port);
int64_t nyx_tcp_send(int32_t fd, const char* data, uint64_t len);
int64_t nyx_tcp_recv(int32_t fd, char* buf, uint64_t len);
void nyx_tcp_close(int32_t fd);
int32_t nyx_tcp_listen(const char* host, int32_t port);
int32_t nyx_tcp_accept(int32_t listen_fd);

/* ── Native SSL / TLS & HTTPS Engine ── */
int32_t nyx_tls_connect(const char* host, int32_t port);
int64_t nyx_tls_send(int32_t handle, const char* data, uint64_t len);
int64_t nyx_tls_recv(int32_t handle, char* buf, uint64_t len);
void nyx_tls_close(int32_t handle);

/* High-level HTTPS request (supports TLS 1.2/1.3, SNI, ALPN, system trust store) */
int64_t nyx_https_fetch(rt_string_t method, rt_string_t url, rt_string_t headers,
                        rt_string_t body, int64_t body_len,
                        rt_string_t resp_buf, int64_t max_resp_len, int64_t* status_out);

int64_t nyx_https_fetch_raw(const char* method, const char* url, const char* headers,
                            const char* body, uint64_t body_len,
                            char* resp_buf, uint64_t max_resp_len, int32_t* status_out);

/* Fetch URL and return stripped readable plain text lines (separated by \n) */
rt_string_t nyx_https_fetch_text(rt_string_t url);
int32_t nyx_get_line_style(rt_string_t text, int32_t line_idx);
rt_string_t nyx_get_line(rt_string_t text, int32_t line_idx);
int32_t nyx_count_lines(rt_string_t text);
rt_string_t nyx_ascii_char(int32_t code);
rt_string_t nyx_string_backspace(rt_string_t s);
int32_t nyx_open_host_browser(rt_string_t url);

#ifdef __cplusplus
}
#endif

#endif
