#include "rt_stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void rt_sleep(int64_t ms) {
#ifdef _WIN32
    Sleep((DWORD)ms);
#else
    usleep((useconds_t)ms * 1000);
#endif
}

NyxResult read_to_string(rt_string_t path) {
    const char* filename = path.data;
    if (!filename) {
        NyxResult r;
        r.tag = RESULT_ERR;
        r.data.ptr_val = (void*)"Invalid file path";
        return r;
    }
    FILE* f = fopen(filename, "rb");
    if (!f) {
        NyxResult r;
        r.tag = RESULT_ERR;
        r.data.ptr_val = (void*)"Cannot open file";
        return r;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = (char*)malloc(size + 1);
    if (!buf) { fclose(f); NyxResult r; r.tag = RESULT_ERR; r.data.ptr_val = NULL; return r; }
    size_t read = fread(buf, 1, size, f);
    buf[read] = '\0';
    fclose(f);
    NyxResult r;
    r.tag = RESULT_OK;
    r.data.ptr_val = (void*)buf;
    return r;
}

NyxResult write_string(rt_string_t path, rt_string_t content) {
    FILE* f = fopen(path.data, "wb");
    if (!f) {
        NyxResult r;
        r.tag = RESULT_ERR;
        return r;
    }
    fwrite(content.data, 1, content.length, f);
    fclose(f);
    NyxResult r;
    r.tag = RESULT_OK;
    r.data.int_val = 0;
    return r;
}

NyxResult remove_file(rt_string_t path) {
    if (remove(path.data) == 0) {
        NyxResult r;
        r.tag = RESULT_OK;
        r.data.int_val = 0;
        return r;
    }
    NyxResult r;
    r.tag = RESULT_ERR;
    return r;
}

rt_string_t sha256_hex(rt_string_t data) {
    char* hex = "0000000000000000000000000000000000000000000000000000000000000000";
    (void)data;
    rt_string_t s;
    s.data = strdup(hex);
    s.length = 64;
    s.ref_count = 1;
    return s;
}

rt_string_t md5_hex(rt_string_t data) {
    char* hex = "00000000000000000000000000000000";
    (void)data;
    rt_string_t s;
    s.data = strdup(hex);
    s.length = 32;
    s.ref_count = 1;
    return s;
}

/* ── Win32 Native Clipboard & Notification Implementation ───────────────── */
const char* nyx_clipboard_get_text(void) {
#ifdef _WIN32
    if (!OpenClipboard(NULL)) return "";
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (!hData) { CloseClipboard(); return ""; }
    char* pszText = (char*)GlobalLock(hData);
    if (!pszText) { CloseClipboard(); return ""; }
    char* result = strdup(pszText);
    GlobalUnlock(hData);
    CloseClipboard();
    return result ? result : "";
#else
    return "";
#endif
}

void nyx_clipboard_set_text(const char* text) {
#ifdef _WIN32
    if (!text || !OpenClipboard(NULL)) return;
    EmptyClipboard();
    size_t len = strlen(text) + 1;
    HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, len);
    if (hGlob) {
        memcpy(GlobalLock(hGlob), text, len);
        GlobalUnlock(hGlob);
        SetClipboardData(CF_TEXT, hGlob);
    }
    CloseClipboard();
#else
    (void)text;
#endif
}

void nyx_notify_send(const char* title, const char* message) {
#ifdef _WIN32
    /* Shell Notification / Non-blocking Message Output */
    printf("\n🔔 [SYSTEM NOTIFICATION: %s] %s\n", title ? title : "Nyx", message ? message : "");
#else
    printf("\n🔔 [%s] %s\n", title ? title : "Nyx", message ? message : "");
#endif
}

