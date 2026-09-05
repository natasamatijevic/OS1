#include "../h/riscv.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/TCB.hpp"
#include "../h/KernelConsole.hpp"
#include "../test/printing.hpp"

extern void userMain();

void wrapper(void* args) {
    Semaphore* sem = (Semaphore*) args;
    userMain();
    sem->signal();
}


int main() {
    Riscv::w_stvec((uint64) Riscv::supervisorTrap);
    TCB::createMain();
    KernelConsole::init();
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    Semaphore* sem = new Semaphore(0);


    TCB* userThread;
    thread_create(&userThread, wrapper, (void*)sem);

    sem->wait();

    __asm__ volatile ("li t3, 0x100000");
    __asm__ volatile ("li t4, 0x5555");
    __asm__ volatile ("sw t4, 0(t3)");

    return 0;
}