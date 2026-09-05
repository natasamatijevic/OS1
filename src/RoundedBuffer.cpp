#include "../h/RoundedBuffer.hpp"

RoundedBuffer::RoundedBuffer(int cap) : capacity(cap), head(0), tail(0), count(0) {
    buffer = new char[capacity];

    itemAvailable = new KernelSemaphore(0);
    spaceAvailable = new KernelSemaphore(capacity);
    mutex = new KernelSemaphore(1);
}

RoundedBuffer::~RoundedBuffer() {
    delete[] buffer;
    sem_close(itemAvailable);
    sem_close(spaceAvailable);
    sem_close(mutex);
}

void RoundedBuffer::putc(char c) {
    spaceAvailable->sem_wait();
    mutex->sem_wait();
    buffer[tail] = c;
    (*(volatile uint8*)CONSOLE_TX_DATA) = c;
    count++;
    tail = (tail + 1) % capacity;
    (*(volatile uint8*)CONSOLE_TX_DATA) = '7';
    mutex->sem_signal();
    (*(volatile uint8*)CONSOLE_TX_DATA) = '8';
    itemAvailable->sem_signal();
    (*(volatile uint8*)CONSOLE_TX_DATA) = '8';
}

void RoundedBuffer::putcNoBlock(char c) {
    if (hasSpace()) {
        buffer[tail] = c;
        count++;
        tail = (tail + 1) % capacity;
        itemAvailable->sem_signal();
    }
}

char RoundedBuffer::getc() {
    (*(volatile uint8*)CONSOLE_TX_DATA) = '1';
    itemAvailable->sem_wait();
    (*(volatile uint8*)CONSOLE_TX_DATA) = '2';
    mutex->sem_wait();
    (*(volatile uint8*)CONSOLE_TX_DATA) = '3';
    char c = buffer[head];
    head = (head + 1) % capacity;
    count--;
    mutex->sem_signal();
    spaceAvailable->sem_signal();
    return c;
}

bool RoundedBuffer::hasSpace() {
    return capacity != count;
}

bool RoundedBuffer::isEmpty() {
    return count == 0;
}
