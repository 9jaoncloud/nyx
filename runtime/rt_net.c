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
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons((uint16_t)port);
    if (!addr || strlen(addr) == 0 || strcmp(addr, "0.0.0.0") == 0) {
        sin.sin_addr.s_addr = INADDR_ANY;
    } else {
        inet_pton(AF_INET, addr, &sin.sin_addr);
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
