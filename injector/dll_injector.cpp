#include <Windows.h>
#include <iostream>

// Entry point
int main() {
    // Enable Russian locale for input/output
    setlocale(LC_ALL, "RU");

    DWORD pid;
    std::cout << "Enter process PID: ";
    std::cin >> pid;

    // Open handle to target process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        std::cerr << "Failed to open process. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    // Path to your DLL
    const char* dllPath = "E:\\projects\\PayloadDLL\\x64\\Release\\PayloadDLL.dll";

    // Allocate memory in target process
    void* allocMem = VirtualAllocEx(hProcess, nullptr, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!allocMem) {
        std::cerr << "Failed to allocate memory. Error code: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    // Write DLL path into allocated memory
    if (!WriteProcessMemory(hProcess, allocMem, dllPath, strlen(dllPath) + 1, nullptr)) {
        std::cerr << "Failed to write DLL path. Error code: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Get address of LoadLibraryA from kernel32.dll
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    FARPROC loadLibAddr = GetProcAddress(hKernel32, "LoadLibraryA");
    if (!loadLibAddr) {
        std::cerr << "Could not find LoadLibraryA." << std::endl;
        VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Create remote thread in the target process to load the DLL
    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)loadLibAddr, allocMem, 0, nullptr);
    if (!hThread) {
        std::cerr << "Failed to create remote thread." << std::endl;
        VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "DLL injected successfully!" << std::endl;

    // Wait for thread to complete
    WaitForSingleObject(hThread, INFINITE);

    // Cleanup
    VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return 0;
}
