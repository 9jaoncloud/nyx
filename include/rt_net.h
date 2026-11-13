#ifndef RT_NET_H
#define RT_NET_H
#include <stdint.h>
#include <stddef.h>
#include "rt_string.h"

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

#ifdef __cplusplus
}
#endif

#endif
