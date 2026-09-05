#ifndef PROJECT_BASE_V1_1_SEMAPHORE_HPP
#define PROJECT_BASE_V1_1_SEMAPHORE_HPP

#include "../h/List.hpp"
#include "../h/TCB.hpp"

struct TCBV {
    TCB* tcb;
    int value;

    TCBV(TCB* tcb, int value) : tcb(tcb), value(value) {};
};

class KernelSemaphore {
private:
    int value = 0;
    List<TCBV> queue;
public:
    KernelSemaphore(unsigned int);
    ~KernelSemaphore();

    int sem_wait();
    int sem_signal();
    int sem_wait_n(unsigned n);
    int sem_signal_n(unsigned n);

};


#endif //PROJECT_BASE_V1_1_SEMAPHORE_HPP
