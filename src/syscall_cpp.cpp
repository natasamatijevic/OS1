#include "../h/syscall_cpp.hpp"

void* operator new (size_t size) {
    return mem_alloc(size);
}

void* operator new[] (size_t size) {
    return mem_alloc(size);
}

void operator delete (void* ptr) {
    mem_free(ptr);
}

void operator delete[] (void* ptr){
    mem_free(ptr);
}

void Thread::threadWrapper(void* arg) {
    Thread* myThread = static_cast<Thread*>(arg);
    if (myThread) {
        myThread->run();
    }
}
Thread::Thread(void (*body)(void *), void *arg)
    : myHandle(nullptr), body(body), arg(arg) {}

Thread::Thread()
    : myHandle(nullptr), body(nullptr), arg(nullptr) {}

Thread::~Thread() {}

int Thread::start() {
    if (myHandle != nullptr) return -1;

    if (body != nullptr) {
        return thread_create(&myHandle, body, arg);
    } else {
        return thread_create(&myHandle, threadWrapper, this);
    }
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

Semaphore::Semaphore(unsigned init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

PeriodicThread::PeriodicThread(time_t period): period(period) {}

void PeriodicThread::terminate() {
    period = 0;
}

void PeriodicThread::run() {
    while (period > 0) {
        this->periodicActivation();
        this->sleep(period);
    }
}

void Console::putc(char c) {
    putc(c);
}

char Console::getc() {
    return getc();
}



