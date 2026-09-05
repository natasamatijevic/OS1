#include "../h/syscall_c.hpp"

void* mem_alloc(size_t size) {
    if (size == 0) return nullptr;
    if (size % MEM_BLOCK_SIZE == 0) {
        size = size / MEM_BLOCK_SIZE;
    }
    else {
        size = size / MEM_BLOCK_SIZE + 1;
    }

    __asm__ volatile ("mv a1, %0" : : "r"(size));
    __asm__ volatile("mv a0, %0" : : "r"(MEM_ALLOC));
    __asm__ volatile("ecall");

    uint64 adr;
    __asm__ volatile("mv %0, a0" : "=r"(adr));
    return (void*) adr;
}

int mem_free (void* ptr) {
    __asm__ volatile ("mv a1, %0" : : "r"(ptr));
    __asm__ volatile("mv a0, %0" : : "r"(MEM_FREE));
    __asm__ volatile("ecall");

    uint64 res;
    __asm__ volatile("mv %0, a0" : "=r"(res));
    return (int) res;
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    void* stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    if (!stack_space) {
        return -1;
    }
    __asm__ volatile("mv a4, %0" : : "r" (stack_space));
    __asm__ volatile("mv a3, %0" : : "r" (arg));
    __asm__ volatile("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (THREAD_CREATE));
    __asm__ volatile("ecall");

    uint64 res;
    __asm__ volatile("mv %0, a0" : "=r" (res));
    return (int) res;
}
int thread_exit() {
    __asm__ volatile("mv a0, %0" : : "r"(THREAD_EXIT));
    __asm__ volatile("ecall");

    uint64 res;
    __asm__ volatile("mv %0, a0" : "=r"(res));
    return (int) res;
}
void thread_dispatch() {
    __asm__ volatile("mv a0, %0" : : "r"(THREAD_DISPATCH));
    __asm__ volatile("ecall");
}

int sem_open (sem_t *handle, unsigned init) {
    __asm__ volatile ("mv a2, %0" : : "r"((int)init));
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(SEM_OPEN));
    __asm__ volatile("ecall");

    uint64 res;
    __asm__ volatile("mv %0, a0" : "=r"(res));
    return (int) res;
}

int sem_close (sem_t handle) {
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(SEM_CLOSE));
    __asm__ volatile("ecall");

    uint64 res;
    __asm__ volatile("mv %0, a0" : "=r"(res));
    return (int) res;
}

int sem_wait (sem_t id) {
    __asm__ volatile ("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(SEM_WAIT));
    __asm__ volatile("ecall");

    uint64 res;
    __asm__ volatile("mv %0, a0" : "=r"(res));
    return (int) res;
}

int sem_signal (sem_t id) {
    __asm__ volatile ("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(SEM_SIGNAL));
    __asm__ volatile("ecall");

    uint64 res;
    __asm__ volatile("mv %0, a0" : "=r"(res));
    return (int) res;
}

int sem_wait_n (sem_t id, unsigned n) {
    __asm__ volatile ("mv a2, %0" : : "r"((int)n));
    __asm__ volatile ("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(SEM_WAIT_N));
    __asm__ volatile("ecall");

    uint64 res;
    __asm__ volatile("mv %0, a0" : "=r"(res));
    return (int) res;
}

int sem_signal_n (sem_t id, unsigned n) {
    __asm__ volatile ("mv a2, %0" : : "r"((int)n));
    __asm__ volatile ("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(SEM_SIGNAL_N));
    __asm__ volatile("ecall");

    uint64 res;
    __asm__ volatile("mv %0, a0" : "=r"(res));
    return (int) res;
}

int time_sleep(time_t time) {
    __asm__ volatile ("mv a1, %0" : : "r"(time));
    __asm__ volatile("mv a0, %0" : : "r"(TIME_SLEEP));
    __asm__ volatile("ecall");

    uint64 res;
    __asm__ volatile("mv %0, a0" : "=r"(res));
    return (int) res;
}

char getc() {
    __asm__ volatile("mv a0, %0" : : "r"(GETC));
    __asm__ volatile ("ecall");

    char  res;
    __asm__ volatile ("mv %0, a0" : "=r"(res));
    return res;
}

void putc(char c) {
    __asm__ volatile ("mv a1, %0" : : "r"(c));
    __asm__ volatile("mv a0, %0" : : "r"(PUTC));
    __asm__ volatile ("ecall");
}

