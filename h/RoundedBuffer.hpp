#ifndef PROJECT_BASE_V1_1_ROUNDEDBUFFER_H
#define PROJECT_BASE_V1_1_ROUNDEDBUFFER_H

#include "syscall_cpp.hpp"
#include "../h/Semaphore.hpp"


class RoundedBuffer {
private:
    int capacity;
    uint64 head = 0;
    uint64 tail = 0;
    volatile int count = 0;
    char* buffer;
    KernelSemaphore* itemAvailable;
    KernelSemaphore* spaceAvailable;
    KernelSemaphore* mutex;

public:
    RoundedBuffer(int cap = 512);
    ~RoundedBuffer();
    char getc();
    void putc(char c);
    void putcNoBlock(char c);
    bool isEmpty();
    bool hasSpace();
};


#endif //PROJECT_BASE_V1_1_ROUNDEDBUFFER_H
