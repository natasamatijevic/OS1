#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../h/TCB.hpp"
#include "../h/Semaphore.hpp"
#include "../h/SleepingList.hpp"
#include "../test/printing.hpp"
#include "../h/KernelConsole.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap() {
    volatile uint64 scause = Riscv::r_scause();
    volatile uint64 sepc = Riscv::r_sepc();
    volatile uint64 sstatus = Riscv::r_sstatus();

    if (scause == USER_ECALL || scause == SYSTEM_ECALL) {
        uint64 code = Riscv::r_user_reg(A0);
        sepc += 4;

        switch (code) {
            case MEM_ALLOC: {
                size_t blockSize = Riscv::r_user_reg(A1);
                void* addr = MemoryAllocator::getInstance().mem_alloc(blockSize);
                Riscv::w_user_reg(A0, (uint64) addr);
                break;
            }
            case MEM_FREE: {
                uint64 addr = Riscv::r_user_reg(A1);
                int ret = MemoryAllocator::getInstance().mem_free((void*) addr);
                Riscv::w_user_reg(A0, ret);
                break;
            }
            case THREAD_CREATE: {
                uint64 handle = Riscv::r_user_reg(A1);
                uint64 start_routine = Riscv::r_user_reg(A2);
                uint64 arg = Riscv::r_user_reg(A3);
                uint64 stack_space = Riscv::r_user_reg(A4);
                int ret = TCB::createThread((TCB**) handle, (void(*)(void*)) start_routine, (void*) arg, (uint64*)stack_space);
                Riscv::w_user_reg(A0, ret);
                break;
            }
            case THREAD_DISPATCH: {
                w_sstatus(sstatus);
                w_sepc(sepc);
                TCB::dispatch();
                break;
            }
            case SEM_OPEN: {
                uint64 handle = Riscv::r_user_reg(A1);
                uint64 init = Riscv::r_user_reg(A2);

                if ((sem_t*) handle == nullptr) {
                    Riscv::w_user_reg(A0, -1);
                }
                else {
                    sem_t sem = new KernelSemaphore((int) init);
                    sem_t* s_handle = (sem_t*) handle;
                    *s_handle = sem;
                    Riscv::w_user_reg(A0, 0);
                }
                break;
            }
            case SEM_CLOSE: {
                uint64 handle = Riscv::r_user_reg(A1);
                if ((sem_t) handle == nullptr) {
                    Riscv::w_user_reg(A0, -1);
                }
                else {
                    sem_t sem = (sem_t) handle;
                    delete sem;
                    Riscv::w_user_reg(A0, 0);
                }
                break;
            }
            case SEM_WAIT: {
                uint64 handle = Riscv::r_user_reg(A1);
                if ((sem_t) handle == nullptr) {
                    Riscv::w_user_reg(A0, -1);
                }
                else {
                    sem_t sem = (sem_t) handle;
                    int res = sem->sem_wait();
                    Riscv::w_user_reg(A0, res);
                }
                break;
            }
            case SEM_SIGNAL: {
                uint64 handle = Riscv::r_user_reg(A1);
                if ((sem_t) handle == nullptr) {
                    Riscv::w_user_reg(A0, -1);
                }
                else {
                    sem_t sem = (sem_t) handle;
                    int res = sem->sem_signal();
                    Riscv::w_user_reg(A0, res);
                }
                break;
            }
            case SEM_WAIT_N: {
                uint64 handle = Riscv::r_user_reg(A1);
                int num = (int)Riscv::r_user_reg(A2);
                if ((sem_t) handle == nullptr) {
                    Riscv::w_user_reg(A0, -1);
                }
                else {
                    sem_t sem = (sem_t) handle;
                    int res = sem->sem_wait_n(num);
                    Riscv::w_user_reg(A0, res);
                }
                break;
            }
            case SEM_SIGNAL_N: {
                uint64 handle = Riscv::r_user_reg(A1);
                int num = (int)Riscv::r_user_reg(A2);
                if ((sem_t) handle == nullptr) {
                    Riscv::w_user_reg(A0, -1);
                }
                else {
                    sem_t sem = (sem_t) handle;
                    int res = sem->sem_signal_n(num);
                    Riscv::w_user_reg(A0, res);
                }
                break;
            }
            case TIME_SLEEP: {
                uint64 time = Riscv::r_user_reg(A1);
                int res = TCB::sleep(time);
                Riscv::w_user_reg(A0, res);
                break;
            }
            case GETC: {
                char res = KernelConsole::getcIn();
                Riscv::w_user_reg(A0, res);
                break;
            }
            case PUTC: {
                char c = Riscv::r_user_reg(A1);
                KernelConsole::putcOut(c);
                break;
            }
        }
    }
    else if (scause == TIMER_INTERRUPT) {
        mc_sip(SIP_SSIE);
        SleepingList::updateSleeping();
        TCB::timeCnt++;
        if (TCB::timeCnt >= TCB::getRunning()->getTimeSlice()) {
            TCB::timeCnt = 0;
            TCB::dispatch();
        }
    }
    else if (scause == CONSOLE_INTERRUPT) {
        int irq = plic_claim();
        if (irq == CONSOLE_IRQ) {
            KernelConsole::interuptHandler();
        }
        plic_complete(irq);
    }
    else if (scause == ILLEGAL_INSTRUCTION) {
        printString("Zabranjen pristupak u zadatom rezimu!");
        printInt(r_scause(), 16);
        printString("\n");
        printInt(r_sepc(), 16);
        printString("\n");
        __asm__ volatile ("li t3, 0x100000");
        __asm__ volatile ("li t4, 0x5555");
        __asm__ volatile ("sw t4, 0(t3)");
    }
    else {
        printInt(r_scause(), 16);
        printString("\n");
        printInt(r_sepc(), 16);
        printString("\n");
        __asm__ volatile ("li t3, 0x100000");
        __asm__ volatile ("li t4, 0x5555");
        __asm__ volatile ("sw t4, 0(t3)");
    }

    w_sepc(sepc);
    w_sstatus(sstatus);
}