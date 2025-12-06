/*
 * Simple test launcher - just shows paths and tries to run wsjtx.exe
 * Compile with: gcc -o test-launcher.exe test-launcher.c
 */

#include <windows.h>
#include <stdio.h>

int main() {
    char exePath[MAX_PATH];
    char libPath[MAX_PATH];
    char wsjtxPath[MAX_PATH];

    // Get the directory where this launcher is located
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    printf("Full exe path: %s\n", exePath);

    // Remove the exe filename to get directory
    char *lastSlash = strrchr(exePath, '\\');
    if (lastSlash) {
        *lastSlash = '\0';
    }
    printf("Exe directory: %s\n", exePath);

    // Build path to lib subdirectory
    snprintf(libPath, MAX_PATH, "%s\\lib", exePath);
    printf("Lib path: %s\n", libPath);

    // Build path to wsjtx.exe in lib
    snprintf(wsjtxPath, MAX_PATH, "%s\\lib\\wsjtx.exe", exePath);
    printf("WSJTX path: %s\n", wsjtxPath);

    // Check if file exists
    DWORD attr = GetFileAttributesA(wsjtxPath);
    if (attr == INVALID_FILE_ATTRIBUTES) {
        printf("ERROR: wsjtx.exe NOT FOUND at %s\n", wsjtxPath);
        printf("GetLastError: %lu\n", GetLastError());
    } else {
        printf("OK: wsjtx.exe exists\n");
    }

    // Try to launch
    printf("\nPress Enter to try launching wsjtx.exe...\n");
    getchar();

    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    char cmdLine[MAX_PATH * 2];
    snprintf(cmdLine, sizeof(cmdLine), "\"%s\"", wsjtxPath);
    printf("Command line: %s\n", cmdLine);

    SetDllDirectoryA(libPath);
    SetCurrentDirectoryA(libPath);

    if (!CreateProcessA(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, libPath, &si, &pi)) {
        printf("CreateProcess FAILED. Error: %lu\n", GetLastError());
    } else {
        printf("CreateProcess SUCCESS\n");
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    printf("\nPress Enter to exit...\n");
    getchar();
    return 0;
}
