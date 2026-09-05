#include "../h/Semaphore.hpp"
#include "../h/Scheduler.hpp"
#include "../h/TCB.hpp"

KernelSemaphore::KernelSemaphore(unsigned value) {
    this->value = static_cast<int>(value);
}

KernelSemaphore::~KernelSemaphore() {
    while (!queue.isEmpty()) {
        TCB* tcb = queue.removeFirst()->tcb;
        tcb->setState(TCB::READY);
        Scheduler::put(tcb);
    }
}

int KernelSemaphore::sem_wait() {
    return sem_wait_n(1);
}

int KernelSemaphore::sem_signal() {
    return sem_signal_n(1);
}

int KernelSemaphore::sem_wait_n(unsigned n) {
    if (n == 0) return -1;

    if (!queue.isEmpty()) {
        TCB::getRunning()->setState(TCB::WAITING);
        TCBV* tcbv = new TCBV(TCB::getRunning(), n);
        this->queue.addLast(tcbv);
        if (queue.isEmpty()) {
            (*(volatile uint8*)CONSOLE_TX_DATA) = 'O';
        }
        if (queue.peekFirst()) {
            (*(volatile uint8*)CONSOLE_TX_DATA) = 'D';
        }

        TCB::dispatch();
        return 0;
    }
    if (this->value >= (int)n) {
        this->value -= n;
        return 0;
    }

    int needed = (int)n - this->value;
    this->value = 0;

    TCB::getRunning()->setState(TCB::WAITING);
    TCBV* tcbv = new TCBV(TCB::getRunning(), needed);
    this->queue.addLast(tcbv);
    TCB::dispatch();

    return 0;
}

int KernelSemaphore::sem_signal_n(unsigned n) {
    if (n == 0) return -1;

    int available = (int)n;
    if (queue.isEmpty()) {
        this->value += available;
        return 0;
    }

    while (available > 0 && !queue.isEmpty()) {
        TCBV* first = queue.peekFirst();


        if (available < first->value) {
            (*(volatile uint8*)CONSOLE_TX_DATA) = '2';
            first->value -= available;
            available = 0;
        } else {
            (*(volatile uint8*)CONSOLE_TX_DATA) = '3';
            available -= first->value;
            queue.removeFirst();

            TCB* tcb = first->tcb;
            //delete first;

            tcb->setState(TCB::READY);
            Scheduler::put(tcb);
        }
        (*(volatile uint8*)CONSOLE_TX_DATA) = '4';
    }

    if (available > 0) {
        this->value += available;
    }
    return 0;

}
