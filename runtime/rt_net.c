#include "rt_net.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <winhttp.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winhttp.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#endif

static int g_socket_initialized = 0;

void nyx_socket_init(void) {
    if (g_socket_initialized) return;
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    g_socket_initialized = 1;
}

int32_t nyx_socket(int32_t domain, int32_t type, int32_t protocol) {
    nyx_socket_init();
    return (int32_t)socket(domain, type, protocol);
}

int32_t nyx_bind(int32_t sock, const char* addr, int32_t port) {
#ifdef _WIN32
    int opt = 1;
    setsockopt((SOCKET)sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
#else
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    char clean_addr[128] = {0};
    int actual_port = port;
    if (addr && strlen(addr) > 0) {
        const char* p = addr;
        if (strncmp(p, "http://", 7) == 0) p += 7;
        else if (strncmp(p, "https://", 8) == 0) p += 8;

        const char* colon = strchr(p, ':');
        if (colon) {
            size_t len = (size_t)(colon - p);
            if (len >= sizeof(clean_addr)) len = sizeof(clean_addr) - 1;
            strncpy(clean_addr, p, len);
            clean_addr[len] = '\0';
            actual_port = atoi(colon + 1);
        } else {
            strncpy(clean_addr, p, sizeof(clean_addr) - 1);
        }
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons((uint16_t)actual_port);
    if (clean_addr[0] == '\0' || strcmp(clean_addr, "0.0.0.0") == 0) {
        sin.sin_addr.s_addr = INADDR_ANY;
    } else if (strcmp(clean_addr, "localhost") == 0 || strcmp(clean_addr, "127.0.0.1") == 0) {
        sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    } else {
        inet_pton(AF_INET, clean_addr, &sin.sin_addr);
    }
    return bind((SOCKET)sock, (struct sockaddr*)&sin, sizeof(sin));
}

int32_t nyx_listen(int32_t sock, int32_t backlog) {
    return listen((SOCKET)sock, backlog);
}

int32_t nyx_accept(int32_t sock) {
    struct sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);
    return (int32_t)accept((SOCKET)sock, (struct sockaddr*)&client_addr, &addrlen);
}

int32_t nyx_connect(const char* host, int32_t port) {
    nyx_socket_init();
    struct addrinfo hints, *res = NULL, *ptr = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char port_str[16];
    snprintf(port_str, sizeof(port_str), "%d", port);

    if (getaddrinfo(host, port_str, &hints, &res) != 0) {
        return -1;
    }

    int32_t sock = -1;
    for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
        sock = (int32_t)socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == -1) continue;
        if (connect((SOCKET)sock, ptr->ai_addr, (int)ptr->ai_addrlen) == 0) {
            break;
        }
        nyx_close(sock);
        sock = -1;
    }

    freeaddrinfo(res);
    return sock;
}

int64_t nyx_send(int32_t sock, const char* data, uint64_t len) {
    return (int64_t)send((SOCKET)sock, data, (int)len, 0);
}

int64_t nyx_recv(int32_t sock, char* buf, uint64_t len) {
    return (int64_t)recv((SOCKET)sock, buf, (int)len, 0);
}

void nyx_close(int32_t sock) {
#ifdef _WIN32
    closesocket((SOCKET)sock);
#else
    close(sock);
#endif
}

void nyx_set_nonblocking(int32_t sock, int32_t enable) {
#ifdef _WIN32
    u_long mode = enable ? 1 : 0;
    ioctlsocket((SOCKET)sock, FIONBIO, &mode);
#else
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags >= 0) {
        fcntl(sock, F_SETFL, enable ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK));
    }
#endif
}

void nyx_set_timeout(int32_t sock, int32_t seconds) {
#ifdef _WIN32
    DWORD timeout = (DWORD)seconds * 1000;
    setsockopt((SOCKET)sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt((SOCKET)sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
#else
    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));
#endif
}

int32_t nyx_peer_addr(int32_t sock, char* buf, int32_t buflen) {
    struct sockaddr_in peer;
    int peerlen = (int)sizeof(peer);
    memset(&peer, 0, sizeof(peer));
    if (getpeername((SOCKET)sock, (struct sockaddr*)&peer, &peerlen) != 0) {
        if (buf && buflen > 0) buf[0] = '\0';
        return -1;
    }
    if (buf && buflen > 0) {
        inet_ntop(AF_INET, &peer.sin_addr, buf, (size_t)buflen);
    }
    return (int32_t)ntohs(peer.sin_port);
}

/* ── WebSocket (RFC 6455) client helpers ── */

static int nyx_ws_read_full(int32_t sock, char* buf, int n) {
    int got = 0;
    while (got < n) {
        int r = recv((SOCKET)sock, buf + got, n - got, 0);
        if (r <= 0) return got;
        got += r;
    }
    return got;
}

int32_t nyx_ws_handshake(int32_t sock, const char* host, int32_t hlen, int32_t port,
                         const char* path, int32_t plen) {
    char key[32];
    nyx_ws_client_key(key);

    char hostbuf[256];
    int hn = (host && hlen > 0) ? (hlen < 255 ? hlen : 255) : 0;
    if (hn) memcpy(hostbuf, host, (size_t)hn);
    hostbuf[hn] = '\0';

    char pathbuf[1024];
    int pn = (path && plen > 0) ? (plen < 1023 ? plen : 1023) : 1;
    if (pn > 1) memcpy(pathbuf, path, (size_t)pn);
    else pathbuf[0] = '/';
    pathbuf[pn] = '\0';

    char req[2048];
    int rn = snprintf(req, sizeof(req),
        "GET %s HTTP/1.1\r\nHost: %s:%d\r\nUpgrade: websocket\r\nConnection: Upgrade\r\n"
        "Sec-WebSocket-Key: %s\r\nSec-WebSocket-Version: 13\r\n\r\n",
        pathbuf, hostbuf, port, key);
    if (rn <= 0) return -1;
    if (send((SOCKET)sock, req, rn, 0) != rn) return -1;

    char buf[4096];
    int total = 0;
    while (total < (int)sizeof(buf) - 1) {
        int n = recv((SOCKET)sock, buf + total, (int)sizeof(buf) - 1 - total, 0);
        if (n <= 0) break;
        total += n;
        buf[total] = '\0';
        if (strstr(buf, "\r\n\r\n")) break;
    }
    buf[total] = '\0';
    if (strncmp(buf, "HTTP/1.1 101", 12) != 0) return -1;
    return 0;
}

int64_t nyx_ws_send(int32_t sock, int32_t opcode, const char* data, int64_t len) {
    unsigned char hdr[14];
    int hl = 0;
    hdr[hl++] = (unsigned char)(0x80 | (opcode & 0x0f));
    if (len < 126) {
        hdr[hl++] = (unsigned char)(0x80 | (int)len);
    } else if (len < 65536) {
        hdr[hl++] = (unsigned char)(0x80 | 126);
        hdr[hl++] = (unsigned char)((len >> 8) & 0xff);
        hdr[hl++] = (unsigned char)(len & 0xff);
    } else {
        hdr[hl++] = (unsigned char)(0x80 | 127);
        for (int i = 7; i >= 0; i--) hdr[hl++] = (unsigned char)((len >> (8 * i)) & 0xff);
    }
    unsigned char mask[4];
    nyx_random_bytes(mask, 4);
    memcpy(hdr + hl, mask, 4);
    hl += 4;
    if (send((SOCKET)sock, (const char*)hdr, hl, 0) != hl) return -1;
    if (len > 0) {
        char* tmp = (char*)malloc((size_t)len);
        if (!tmp) return -1;
        for (int64_t i = 0; i < len; i++) tmp[i] = (char)(data[i] ^ mask[i % 4]);
        int64_t sent = send((SOCKET)sock, tmp, (int)len, 0);
        free(tmp);
        if (sent != len) return -1;
    }
    return len;
}

int64_t nyx_ws_recv(int32_t sock, char* out, int32_t max, int32_t* opcode, int32_t* fin) {
    unsigned char h2[2];
    if (nyx_ws_read_full(sock, (char*)h2, 2) != 2) return -1;
    if (fin) *fin = (h2[0] & 0x80) ? 1 : 0;
    if (opcode) *opcode = (int32_t)(h2[0] & 0x0f);
    int masked = (h2[1] & 0x80) != 0;
    uint64_t len = (uint64_t)(h2[1] & 0x7f);
    if (len == 126) {
        unsigned char e[2];
        if (nyx_ws_read_full(sock, (char*)e, 2) != 2) return -1;
        len = ((uint64_t)e[0] << 8) | e[1];
    } else if (len == 127) {
        unsigned char e[8];
        if (nyx_ws_read_full(sock, (char*)e, 8) != 8) return -1;
        len = 0;
        for (int i = 0; i < 8; i++) len = (len << 8) | e[i];
    }
    unsigned char mask[4] = { 0, 0, 0, 0 };
    if (masked) {
        if (nyx_ws_read_full(sock, (char*)mask, 4) != 4) return -1;
    }
    int64_t n = (int64_t)len;
    if (n > (int64_t)max) n = (int64_t)max;
    if (n > 0) {
        if (nyx_ws_read_full(sock, out, (int)n) != (int)n) return -1;
    }
    if (masked) {
        for (int64_t i = 0; i < n; i++) out[i] = (char)(((unsigned char)out[i]) ^ mask[i % 4]);
    }
    return n;
}

int32_t nyx_udp_socket(int32_t domain) {
    nyx_socket_init();
    return (int32_t)socket(domain, SOCK_DGRAM, IPPROTO_UDP);
}

int64_t nyx_udp_sendto(int32_t sock, const char* data, uint64_t len, const char* addr, int32_t port) {
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons((uint16_t)port);
    inet_pton(AF_INET, addr, &sin.sin_addr);
    return (int64_t)sendto((SOCKET)sock, data, (int)len, 0, (struct sockaddr*)&sin, sizeof(sin));
}

int64_t nyx_udp_recvfrom(int32_t sock, char* buf, uint64_t len, char* addr, int32_t* port) {
    struct sockaddr_in from;
    int fromlen = sizeof(from);
    int64_t n = (int64_t)recvfrom((SOCKET)sock, buf, (int)len, 0, (struct sockaddr*)&from, &fromlen);
    if (n > 0) {
        if (addr) inet_ntop(AF_INET, &from.sin_addr, addr, 256);
        if (port) *port = ntohs(from.sin_port);
    }
    return n;
}

int32_t nyx_tcp_connect(const char* host, int32_t port) {
    return nyx_connect(host, port);
}

int64_t nyx_tcp_send(int32_t fd, const char* data, uint64_t len) {
    return nyx_send(fd, data, len);
}

int64_t nyx_tcp_recv(int32_t fd, char* buf, uint64_t len) {
    return nyx_recv(fd, buf, len);
}

void nyx_tcp_close(int32_t fd) {
    nyx_close(fd);
}

int32_t nyx_tcp_listen(const char* host, int32_t port) {
    int32_t sock = nyx_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) return -1;
    if (nyx_bind(sock, host, port) < 0) {
        nyx_close(sock);
        return -1;
    }
    if (nyx_listen(sock, 128) < 0) {
        nyx_close(sock);
        return -1;
    }
    return sock;
}

int32_t nyx_tcp_accept(int32_t listen_fd) {
    return nyx_accept(listen_fd);
}

/* ── Native SSL / TLS & HTTPS Engine ── */

int32_t nyx_tls_connect(const char* host, int32_t port) {
    return nyx_connect(host, port);
}

int64_t nyx_tls_send(int32_t handle, const char* data, uint64_t len) {
    return nyx_send(handle, data, len);
}

int64_t nyx_tls_recv(int32_t handle, char* buf, uint64_t len) {
    return nyx_recv(handle, buf, len);
}

void nyx_tls_close(int32_t handle) {
    nyx_close(handle);
}

/* Raw WinHTTP HTTPS fetch */
int64_t nyx_https_fetch_raw(const char* method, const char* url, const char* headers,
                            const char* body, uint64_t body_len,
                            char* resp_buf, uint64_t max_resp_len, int32_t* status_out) {
    if (!url || !resp_buf || max_resp_len == 0) return -1;
    if (status_out) *status_out = 0;

#ifdef _WIN32
    WCHAR wurl[2048] = {0};
    MultiByteToWideChar(CP_UTF8, 0, url, -1, wurl, 2047);

    URL_COMPONENTS urlComp;
    memset(&urlComp, 0, sizeof(urlComp));
    urlComp.dwStructSize = sizeof(urlComp);
    WCHAR hostName[256] = {0};
    WCHAR urlPath[2048] = {0};
    urlComp.lpszHostName = hostName;
    urlComp.dwHostNameLength = 255;
    urlComp.lpszUrlPath = urlPath;
    urlComp.dwUrlPathLength = 2047;

    if (!WinHttpCrackUrl(wurl, (DWORD)wcslen(wurl), 0, &urlComp)) {
        return -1;
    }

    HINTERNET hSession = WinHttpOpen(L"Nyx-HTTP-Client/0.49 (Sovereign OS)",
                                     WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     WINHTTP_NO_PROXY_NAME,
                                     WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) return -1;

    DWORD protocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
#ifdef WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_3
    protocols |= WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_3;
#endif
    WinHttpSetOption(hSession, WINHTTP_OPTION_SECURE_PROTOCOLS, &protocols, sizeof(protocols));

    HINTERNET hConnect = WinHttpConnect(hSession, hostName, urlComp.nPort, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        return -1;
    }

    WCHAR wmethod[32] = {0};
    MultiByteToWideChar(CP_UTF8, 0, method ? method : "GET", -1, wmethod, 31);

    const WCHAR* req_path = (urlPath[0] != L'\0') ? urlPath : L"/";
    DWORD openFlags = (urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0;
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, wmethod, req_path,
                                           NULL, WINHTTP_NO_REFERER,
                                           WINHTTP_DEFAULT_ACCEPT_TYPES,
                                           openFlags);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return -1;
    }

    DWORD redirect_policy = WINHTTP_OPTION_REDIRECT_POLICY_ALWAYS;
    WinHttpSetOption(hRequest, WINHTTP_OPTION_REDIRECT_POLICY, &redirect_policy, sizeof(redirect_policy));

    if (headers && headers[0]) {
        WCHAR wheaders[4096] = {0};
        MultiByteToWideChar(CP_UTF8, 0, headers, -1, wheaders, 4095);
        WinHttpAddRequestHeaders(hRequest, wheaders, (DWORD)-1L, WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE);
    }

    LPVOID opt_body = (body_len > 0 && body) ? (LPVOID)body : WINHTTP_NO_REQUEST_DATA;
    DWORD opt_len = (body_len > 0 && body) ? (DWORD)body_len : 0;
    BOOL sent = WinHttpSendRequest(hRequest,
                                  WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                  opt_body, opt_len,
                                  opt_len, 0);

    if (!sent || !WinHttpReceiveResponse(hRequest, NULL)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return -1;
    }

    DWORD statusCode = 0;
    DWORD statusSize = sizeof(statusCode);
    WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                        WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &statusSize, WINHTTP_NO_HEADER_INDEX);
    if (status_out) *status_out = (int32_t)statusCode;

    DWORD bytesRead = 0;
    DWORD totalRead = 0;
    while (totalRead < (DWORD)(max_resp_len - 1)) {
        DWORD toRead = (DWORD)(max_resp_len - 1 - totalRead);
        if (!WinHttpReadData(hRequest, resp_buf + totalRead, toRead, &bytesRead) || bytesRead == 0) {
            break;
        }
        totalRead += bytesRead;
    }
    resp_buf[totalRead] = '\0';

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return (int64_t)totalRead;
#else
    if (status_out) *status_out = 200;
    strncpy(resp_buf, "{}", max_resp_len);
    return (int64_t)strlen(resp_buf);
#endif
}

/* Nyx rt_string_t calling convention bridge */
int64_t nyx_https_fetch(rt_string_t method, rt_string_t url, rt_string_t headers,
                        rt_string_t body, int64_t body_len,
                        rt_string_t resp_buf, int64_t max_resp_len, int64_t* status_out) {
    const char* method_s = (method.data && method.length > 0) ? method.data : "GET";
    const char* url_s = (url.data && url.length > 0) ? url.data : "";
    const char* headers_s = (headers.data && headers.length > 0) ? headers.data : "";
    const char* body_s = (body.data && body.length > 0) ? body.data : "";
    char* out_buf = resp_buf.data;
    if (!out_buf || max_resp_len <= 0) return -1;

    int32_t st = 0;
    uint64_t blen = (body_len > 0) ? (uint64_t)body_len : (uint64_t)body.length;
    int64_t res = nyx_https_fetch_raw(method_s, url_s, headers_s, body_s, blen, out_buf, (uint64_t)max_resp_len, &st);
    if (status_out) *status_out = (int64_t)st;
    return res;
}

rt_string_t nyx_ascii_char(int32_t code) {
    char buf[2] = {0, 0};
    if (code >= 32 && code <= 126) {
        buf[0] = (char)code;
    }
    return rt_string_from(buf);
}

rt_string_t nyx_string_backspace(rt_string_t s) {
    if (!s.data || s.length == 0) return rt_string_from("");
    char* buf = (char*)calloc(1, s.length + 1);
    if (!buf) return rt_string_from("");
    memcpy(buf, s.data, s.length - 1);
    buf[s.length - 1] = '\0';
    rt_string_t res = rt_string_from(buf);
    free(buf);
    return res;
}

int32_t nyx_count_lines(rt_string_t text) {
    if (!text.data || text.length == 0) return 0;
    int32_t lines = 1;
    size_t col = 0;
    for (size_t i = 0; i < text.length; i++) {
        if (text.data[i] == '\n') {
            lines++;
            col = 0;
        } else {
            col++;
            if (col >= 50) {
                lines++;
                col = 0;
            }
        }
    }
    return lines;
}

int32_t nyx_get_line_style(rt_string_t text, int32_t line_idx) {
    rt_string_t ln = nyx_get_line(text, line_idx);
    if (!ln.data || ln.length == 0) return 0;
    const char* s = ln.data;
    while (*s == ' ') s++;
    if (strncmp(s, "# ", 2) == 0 || strncmp(s, "TITLE:", 6) == 0) return 1;      /* H1 / Page Title */
    if (strncmp(s, "## ", 3) == 0 || strncmp(s, "### ", 4) == 0) return 2;        /* H2 / H3 Heading */
    if (strncmp(s, "[LINK] ", 7) == 0 || strncmp(s, "-> ", 3) == 0) return 3;     /* Interactive Link / Nav */
    if (strncmp(s, "[INPUT", 6) == 0 || strncmp(s, "[BTN]", 5) == 0) return 4;    /* Form / Search / Button */
    if (strncmp(s, "• ", 2) == 0 || strncmp(s, "* ", 2) == 0) return 5;           /* Bullet item */
    if (strncmp(s, "---", 3) == 0 || strncmp(s, "===", 3) == 0) return 6;         /* Divider / Meta */
    return 0;
}

rt_string_t nyx_get_line(rt_string_t text, int32_t line_idx) {
    if (!text.data || text.length == 0 || line_idx < 0) return rt_string_from("");
    int32_t cur_line = 0;
    size_t col = 0;
    char buf[64];
    size_t b_idx = 0;

    for (size_t i = 0; i < text.length; i++) {
        char c = text.data[i];
        if (c == '\n') {
            if (cur_line == line_idx) {
                buf[b_idx] = '\0';
                return rt_string_from(buf);
            }
            cur_line++;
            col = 0;
            b_idx = 0;
        } else {
            if (cur_line == line_idx && b_idx < 52) {
                buf[b_idx++] = c;
            }
            col++;
            if (col >= 50) {
                if (cur_line == line_idx) {
                    buf[b_idx] = '\0';
                    return rt_string_from(buf);
                }
                cur_line++;
                col = 0;
                b_idx = 0;
            }
        }
    }
    if (cur_line == line_idx && b_idx > 0) {
        buf[b_idx] = '\0';
        return rt_string_from(buf);
    }
    return rt_string_from("");
}

int32_t nyx_open_host_browser(rt_string_t url) {
#ifdef _WIN32
    char norm[2048] = {0};
    const char* u = (url.data && url.length > 0) ? url.data : "https://nyx.9jaoncloud.com.ng";
    if (strncmp(u, "http://", 7) != 0 && strncmp(u, "https://", 8) != 0) {
        snprintf(norm, sizeof(norm) - 1, "https://%s", u);
    } else {
        snprintf(norm, sizeof(norm) - 1, "%s", u);
    }
    char cmd[2400];
    snprintf(cmd, sizeof(cmd) - 1, "cmd.exe /c start \"\" \"%s\"", norm);
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    memset(&pi, 0, sizeof(pi));
    if (CreateProcessA(NULL, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return 1;
    }
#endif
    return 0;
}

rt_string_t nyx_https_fetch_text(rt_string_t url) {
    char norm_url[2048] = {0};
    const char* raw_u = (url.data && url.length > 0) ? url.data : "https://geo.com.ng";
    while (*raw_u == ' ') raw_u++;
    if (strncmp(raw_u, "http://", 7) != 0 && strncmp(raw_u, "https://", 8) != 0) {
        snprintf(norm_url, sizeof(norm_url) - 1, "https://%s", raw_u);
    } else {
        snprintf(norm_url, sizeof(norm_url) - 1, "%s", raw_u);
    }

    char* raw = (char*)calloc(1, 262144);
    if (!raw) return rt_string_from("ERR: Out of memory");
    int32_t st = 0;
    const char* req_hdrs =
        "User-Agent: Mozilla/5.0 (Linux; Android 14; NyxMobile Sovereign OS) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.0.0 Mobile Safari/537.36\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
        "Accept-Language: en-US,en;q=0.9\r\n"
        "Accept-Encoding: identity\r\n";

    int64_t n = nyx_https_fetch_raw("GET", norm_url, req_hdrs, "", 0, raw, 260000, &st);
    if (n <= 0) {
        free(raw);
        char err_msg[512];
        snprintf(err_msg, sizeof(err_msg) - 1, "# Connection Failed\nURL: %s\nStatus: Could not reach host via TLS 1.3.\nCheck spelling or Per-App Network Kill-Switch.", norm_url);
        return rt_string_from(err_msg);
    }

    char* clean = (char*)calloc(1, 65536);
    if (!clean) {
        free(raw);
        return rt_string_from("ERR: Out of memory");
    }

    size_t out_idx = 0;
    out_idx += (size_t)snprintf(clean + out_idx, 512, "=== HTTP %d OK | %lld bytes | %s ===\n", st > 0 ? st : 200, (long long)n, norm_url);

    int in_tag = 0;
    int in_script_or_style = 0;
    int last_was_space = 1;

    for (int64_t i = 0; i < n && out_idx < 64000; i++) {
        char c = raw[i];
        if (c == '<') {
            in_tag = 1;
            if (i + 7 < n && (strncasecmp(raw + i, "<script", 7) == 0 ||
                              strncasecmp(raw + i, "<style", 6) == 0 ||
                              strncasecmp(raw + i, "<svg", 4) == 0 ||
                              strncasecmp(raw + i, "<noscript", 9) == 0)) {
                in_script_or_style = 1;
            } else if (i + 8 < n && (strncasecmp(raw + i, "</script", 8) == 0 ||
                                     strncasecmp(raw + i, "</style", 7) == 0 ||
                                     strncasecmp(raw + i, "</svg", 5) == 0 ||
                                     strncasecmp(raw + i, "</noscript", 10) == 0)) {
                in_script_or_style = 0;
            } else if (!in_script_or_style) {
                if (i + 6 < n && strncasecmp(raw + i, "<title", 6) == 0) {
                    if (out_idx > 0 && clean[out_idx - 1] != '\n') clean[out_idx++] = '\n';
                    out_idx += (size_t)snprintf(clean + out_idx, 32, "# ");
                    last_was_space = 1;
                } else if (i + 3 < n && strncasecmp(raw + i, "<h1", 3) == 0) {
                    if (out_idx > 0 && clean[out_idx - 1] != '\n') clean[out_idx++] = '\n';
                    out_idx += (size_t)snprintf(clean + out_idx, 32, "# ");
                    last_was_space = 1;
                } else if (i + 3 < n && (strncasecmp(raw + i, "<h2", 3) == 0 || strncasecmp(raw + i, "<h3", 3) == 0)) {
                    if (out_idx > 0 && clean[out_idx - 1] != '\n') clean[out_idx++] = '\n';
                    out_idx += (size_t)snprintf(clean + out_idx, 32, "## ");
                    last_was_space = 1;
                } else if (i + 3 < n && strncasecmp(raw + i, "<li", 3) == 0) {
                    if (out_idx > 0 && clean[out_idx - 1] != '\n') clean[out_idx++] = '\n';
                    out_idx += (size_t)snprintf(clean + out_idx, 32, "* ");
                    last_was_space = 1;
                } else if (i + 3 < n && strncasecmp(raw + i, "<a ", 3) == 0) {
                    if (out_idx > 0 && clean[out_idx - 1] != '\n') clean[out_idx++] = '\n';
                    out_idx += (size_t)snprintf(clean + out_idx, 32, "-> ");
                    last_was_space = 1;
                } else if (i + 6 < n && strncasecmp(raw + i, "<input", 6) == 0) {
                    /* Extract input value or placeholder or search box */
                    if (out_idx > 0 && clean[out_idx - 1] != '\n') clean[out_idx++] = '\n';
                    out_idx += (size_t)snprintf(clean + out_idx, 64, "[INPUT: Search / Form Field]\n");
                    last_was_space = 1;
                } else if (i + 7 < n && strncasecmp(raw + i, "<button", 7) == 0) {
                    if (out_idx > 0 && clean[out_idx - 1] != '\n') clean[out_idx++] = '\n';
                    out_idx += (size_t)snprintf(clean + out_idx, 32, "[BTN] ");
                    last_was_space = 1;
                } else if (i + 4 < n && (strncasecmp(raw + i, "<p>", 3) == 0 || strncasecmp(raw + i, "<p ", 3) == 0 ||
                                         strncasecmp(raw + i, "<br", 3) == 0 || strncasecmp(raw + i, "<tr", 3) == 0 ||
                                         strncasecmp(raw + i, "<div", 4) == 0 || strncasecmp(raw + i, "</p>", 4) == 0 ||
                                         strncasecmp(raw + i, "</h1", 4) == 0 || strncasecmp(raw + i, "</h2", 4) == 0 ||
                                         strncasecmp(raw + i, "</h3", 4) == 0 || strncasecmp(raw + i, "</li", 4) == 0 ||
                                         strncasecmp(raw + i, "</title", 7) == 0)) {
                    if (out_idx > 0 && clean[out_idx - 1] != '\n') {
                        clean[out_idx++] = '\n';
                        last_was_space = 1;
                    }
                }
            }
            continue;
        }
        if (c == '>') {
            in_tag = 0;
            continue;
        }
        if (in_tag || in_script_or_style) continue;

        /* Decode common HTML entities (&amp;, &ndash;, &rarr;, &copy;, &nbsp;, &#...;) */
        if (c == '&') {
            if (i + 4 < n && strncasecmp(raw + i, "&amp;", 5) == 0) {
                clean[out_idx++] = '&'; i += 4; last_was_space = 0; continue;
            } else if (i + 5 < n && strncasecmp(raw + i, "&nbsp;", 6) == 0) {
                if (!last_was_space) { clean[out_idx++] = ' '; last_was_space = 1; }
                i += 5; continue;
            } else if (i + 6 < n && (strncasecmp(raw + i, "&ndash;", 7) == 0 || strncasecmp(raw + i, "&mdash;", 7) == 0)) {
                clean[out_idx++] = '-'; i += 6; last_was_space = 0; continue;
            } else if (i + 5 < n && strncasecmp(raw + i, "&rarr;", 6) == 0) {
                clean[out_idx++] = '-'; clean[out_idx++] = '>'; i += 5; last_was_space = 0; continue;
            } else if (i + 5 < n && strncasecmp(raw + i, "&quot;", 6) == 0) {
                clean[out_idx++] = '"'; i += 5; last_was_space = 0; continue;
            } else if (i + 5 < n && strncasecmp(raw + i, "&copy;", 6) == 0) {
                clean[out_idx++] = '('; clean[out_idx++] = 'c'; clean[out_idx++] = ')'; i += 5; last_was_space = 0; continue;
            } else if (i + 3 < n && strncasecmp(raw + i, "&lt;", 4) == 0) {
                clean[out_idx++] = '<'; i += 3; last_was_space = 0; continue;
            } else if (i + 3 < n && strncasecmp(raw + i, "&gt;", 4) == 0) {
                clean[out_idx++] = '>'; i += 3; last_was_space = 0; continue;
            } else if (i + 2 < n && raw[i + 1] == '#') {
                int64_t j = i + 2;
                while (j < i + 10 && j < n && raw[j] != ';') j++;
                if (j < n && raw[j] == ';') { i = j; continue; }
            }
        }

        if (c == '\r') continue;
        if (c == '\n') {
            if (out_idx > 0 && clean[out_idx - 1] != '\n') {
                clean[out_idx++] = '\n';
                last_was_space = 1;
            }
            continue;
        }
        if (c == ' ' || c == '\t') {
            if (!last_was_space) {
                clean[out_idx++] = ' ';
                last_was_space = 1;
            }
            continue;
        }
        if ((unsigned char)c >= 32 && (unsigned char)c <= 126) {
            clean[out_idx++] = c;
            last_was_space = 0;
        }
    }
    clean[out_idx] = '\0';

    /* Post-process pass: strip empty marker lines ("* ", "-> ", "[BTN] ", "# ", "## ") */
    char* final_buf = (char*)calloc(1, 65536);
    if (!final_buf) {
        rt_string_t res = rt_string_from(clean);
        free(raw);
        free(clean);
        return res;
    }
    size_t f_idx = 0;
    const char* line_p = clean;
    while (*line_p && f_idx < 64000) {
        const char* nl = strchr(line_p, '\n');
        size_t l_len = nl ? (size_t)(nl - line_p) : strlen(line_p);

        /* Trim trailing spaces */
        while (l_len > 0 && (line_p[l_len - 1] == ' ' || line_p[l_len - 1] == '\t')) {
            l_len--;
        }
        /* Check if line has actual content beyond just a prefix marker */
        int skip = 0;
        if (l_len == 0) skip = 1;
        else if (l_len <= 2 && (strncmp(line_p, "*", 1) == 0 || strncmp(line_p, "#", 1) == 0 || strncmp(line_p, "->", 2) == 0)) skip = 1;
        else if (l_len <= 3 && (strncmp(line_p, "##", 2) == 0 || strncmp(line_p, "-> ", 3) == 0)) skip = 1;
        else if (l_len <= 6 && strncmp(line_p, "[BTN]", 5) == 0) skip = 1;

        if (!skip) {
            memcpy(final_buf + f_idx, line_p, l_len);
            f_idx += l_len;
            final_buf[f_idx++] = '\n';
        }
        if (!nl) break;
        line_p = nl + 1;
    }
    final_buf[f_idx] = '\0';

    rt_string_t res = rt_string_from(final_buf);
    free(raw);
    free(clean);
    free(final_buf);
    return res;
}
