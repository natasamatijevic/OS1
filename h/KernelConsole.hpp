#ifndef PROJECT_BASE_V1_1_KERNELCONSOLE_H
#define PROJECT_BASE_V1_1_KERNELCONSOLE_H

#include "../h/RoundedBuffer.hpp"

class KernelConsole {
public:
    static RoundedBuffer* inputBuffer;
    static RoundedBuffer* outputBuffer;
    static const int BUFFER_SIZE = 512;

    static void ConsoleWorker(void* arg);

public:
    static void init();
    static void interuptHandler();

    static char getcIn();
    static void putcOut(char c);
    static char getcOut();
};


#endif //PROJECT_BASE_V1_1_KERNELCONSOLE_H
