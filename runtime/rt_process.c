#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
int nyx_get_argc(void) { return __argc; }
const char* nyx_get_argv(int idx) { return (idx >= 0 && idx < __argc) ? __argv[idx] : ""; }
int64_t nyx_process_spawn(const char* cmd) {
    STARTUPINFOA si = {0}; PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);
    char buf[4096]; snprintf(buf, sizeof(buf), "cmd /c %s", cmd);
    if (CreateProcessA(NULL, buf, NULL, NULL, 0, 0, NULL, NULL, &si, &pi))
        return (int64_t)(intptr_t)pi.hProcess;
    return -1;
}
int nyx_process_wait(int64_t pid) {
    HANDLE h = (HANDLE)(intptr_t)pid;
    WaitForSingleObject(h, INFINITE);
    DWORD code; GetExitCodeProcess(h, &code); CloseHandle(h);
    return (int)code;
}
int nyx_process_kill(int64_t pid) {
    return TerminateProcess((HANDLE)(intptr_t)pid, 1) ? 0 : -1;
}
void nyx_exit(int code) { ExitProcess(code); }
#else
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
extern int __argc;
extern char** __argv;
int nyx_get_argc(void) { return __argc; }
const char* nyx_get_argv(int idx) { return (idx >= 0 && idx < __argc) ? __argv[idx] : ""; }
int64_t nyx_process_spawn(const char* cmd) {
    pid_t pid = fork();
    if (pid == 0) { execl("/bin/sh", "sh", "-c", cmd, NULL); _exit(127); }
    return (int64_t)pid;
}
int nyx_process_wait(int64_t pid) {
    int status; waitpid((pid_t)pid, &status, 0);
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    return -1;
}
int nyx_process_kill(int64_t pid) {
    return kill((pid_t)pid, SIGTERM);
}
void nyx_exit(int code) { _exit(code); }
#endif
