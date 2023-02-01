//
// Created by Ascarre on 01-02-2023.
//

#ifndef PRIVATE_DUMPER_MEMORY_H
#define PRIVATE_DUMPER_MEMORY_H

#include <sys/syscall.h>
#include <sys/uio.h>
#include <unistd.h>
#include <iostream>
#include <dirent.h>
#include <fstream>

using namespace std;

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

// Signed base types.
typedef signed char int8;
typedef signed short int int16;
typedef signed int int32;
typedef signed long long int64;

typedef unsigned short UTF16;

#define INRANGE(x, a, b)        (x >= a && x <= b)
#define getBits(x)              (INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte(x)              (getBits(x[0]) << 4 | getBits(x[1]))

pid_t ProcessId = -1;
uintptr_t ModuleBase = 0;
uintptr_t ModuleEnd = 0;

#if defined(__arm__)
int process_vm_readv_syscall = 376;
int process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
int process_vm_readv_syscall = 270;
int process_vm_writev_syscall = 271;
#elif defined(__i386__)
int process_vm_readv_syscall = 347;
int process_vm_writev_syscall = 348;
#else
int process_vm_readv_syscall = 310;
int process_vm_writev_syscall = 311;
#endif

bool ProcessVirtualMemory(void *address, void *buffer, size_t size, bool iswrite) {
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;

    if (ProcessId < 0) {
        return false;
    }

    ssize_t bytes = syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall),
                            ProcessId, local, 1, remote, 1, 0);
    return bytes == size;
}

bool PVM_Read(void *address, void *buffer, size_t size) {
    return ProcessVirtualMemory(address, buffer, size, false);
}

template<typename T>
T Read(uintptr_t address) {
    T data;
    PVM_Read(reinterpret_cast<void *>(address), reinterpret_cast<void *>(&data), sizeof(T));
    return data;
}

string ReadString(uintptr_t address, unsigned int size) {
    char *data = new char[size];
    memset(data, '\0', size);

    for (int i = 0; i < size; i++) {
        PVM_Read((void *) (address + (sizeof(char) * i)), (void *) (&data[0] + i), sizeof(char));
        if (data[i] == 0x0) {
            break;
        }
    }

    string name(data);
    name.shrink_to_fit();
    return name;
}

string ReadString2(uintptr_t address, unsigned int size) {
    string name(size, '\0');
    PVM_Read((void *) address, (void *) name.data(), size * sizeof(char));
    name.shrink_to_fit();
    return name;
}

template<typename T>
T *ReadArray(uintptr_t address, unsigned int size) {
    T data[size];
    T *ptr = data;
    PVM_Read(reinterpret_cast<void *>(address), reinterpret_cast<void *>(ptr), (sizeof(T) * size));
    return ptr;
}

pid_t FindProcessId(const char *ProcessName) {
    int id;
    pid_t pid = -1;
    DIR *dir;
    FILE *fp;
    char filename[32];
    char cmdline[256];

    struct dirent *entry;
    if (ProcessName == NULL) {
        return -1;
    }
    dir = opendir("/proc");
    if (dir == NULL) {
        return -1;
    }
    while ((entry = readdir(dir)) != NULL) {
        id = atoi(entry->d_name);
        if (id != 0) {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp) {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);

                if (strcmp(ProcessName, cmdline) == 0) {
                    pid = id;
                    break;
                }
            }
        }
    }

    closedir(dir);
    return pid;
}

uintptr_t GetModuleBase(int ProcessId, const char* ModuleName) {
    FILE *fp;
    uintptr_t addr = 0;
    char filename[32], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", ProcessId);
    fp = fopen(filename, "rt");
    if (fp != nullptr) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, ModuleName)) {
#if defined(__LP64__)
                sscanf(buffer, "%lx-%*s", &addr);
#else
                sscanf(buffer, "%x-%*s", &addr);
#endif
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}

uintptr_t GetModuleEnd(int ProcessId, const char* ModuleName) {
    FILE *fp;
    uintptr_t temp = 0, addr = 0;
    char filename[32], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", ProcessId);
    fp = fopen(filename, "rt");
    if (fp != nullptr) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, ModuleName)) {
#if defined(__LP64__)
                sscanf(buffer, "%lx-%lx %*s", &temp, &addr);
#else
                sscanf(buffer, "%x-%x %*s", &temp, &addr);
#endif
            }
        }
        fclose(fp);
    }
    return addr;
}

uintptr_t FindPattern(int ProcessId, const char* ModuleName, const char *pattern) {
    auto start = GetModuleBase(ProcessId, ModuleName);
    if (!start)
        return 0;

    auto end = GetModuleEnd(ProcessId, ModuleName);
    if (!end)
        return 0;

    auto curPat = reinterpret_cast<const unsigned char *>(pattern);
    uintptr_t firstMatch = 0;
    for (uintptr_t pCur = start; pCur < end; ++pCur) {
        if (*(uint8_t *) curPat == (uint8_t) '\?' || *(uint8_t *) pCur == getByte(curPat)) {
            if (!firstMatch) {
                firstMatch = pCur;
            }
            curPat += (*(uint16_t *) curPat == (uint16_t) '\?\?' ||
                       *(uint8_t *) curPat != (uint8_t) '\?') ? 2 : 1;
            if (!*curPat) {
                return firstMatch;
            }
            curPat++;
            if (!*curPat) {
                return firstMatch;
            }
        } else if (firstMatch) {
            pCur = firstMatch;
            curPat = reinterpret_cast<const unsigned char *>(pattern);
            firstMatch = 0;
        }
    }
    return 0;
}

#endif //PRIVATE_DUMPER_MEMORY_H
