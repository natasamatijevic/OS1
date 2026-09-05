#include "../h/KernelConsole.hpp"
#include "../lib/hw.h"
#include "../h/TCB.hpp"
#include "../h/MemoryAllocator.hpp"

RoundedBuffer* KernelConsole::inputBuffer = nullptr;
RoundedBuffer* KernelConsole::outputBuffer = nullptr;

void KernelConsole::init() {

    inputBuffer = new RoundedBuffer(BUFFER_SIZE);
    outputBuffer = new RoundedBuffer(BUFFER_SIZE);

    TCB* workerHandle = nullptr;
    void* stack = MemoryAllocator::getInstance().mem_alloc(DEFAULT_STACK_SIZE);
    TCB::createThread(&workerHandle, &KernelConsole::ConsoleWorker, nullptr, (uint64*)stack);

}

char KernelConsole::getcIn() {
    return inputBuffer->getc();
}

char KernelConsole::getcOut() {
    return outputBuffer->getc();
}

void KernelConsole::putcOut(char c) {
    outputBuffer->putc(c);
}

void KernelConsole::ConsoleWorker(void *arg) {
    (*(volatile uint8*)CONSOLE_TX_DATA) = '1';
    volatile uint8* dataR = (uint8*)CONSOLE_TX_DATA;
    volatile uint8* statusR = (uint8*)CONSOLE_STATUS;
    while (true) {
        char c = getcOut();
        while ((*statusR & CONSOLE_TX_STATUS_BIT) == 0) {
            (*(volatile uint8*)CONSOLE_TX_DATA) = '2';
        }
        (*(volatile uint8*)CONSOLE_TX_DATA) = c;
        *dataR = c;
    }
}

void KernelConsole::interuptHandler() {
    uint8* volatile data_input_reg = (uint8*)CONSOLE_RX_DATA;
    uint8* volatile status_reg = (uint8*)CONSOLE_STATUS;
    while ((*status_reg & CONSOLE_RX_STATUS_BIT )> 0 ){
        char c = (*data_input_reg);
        inputBuffer->putcNoBlock(c);
    }
}
