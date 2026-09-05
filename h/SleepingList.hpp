#ifndef PROJECT_BASE_V1_1_SLEEPINGLIST_H
#define PROJECT_BASE_V1_1_SLEEPINGLIST_H

#include "TCB.hpp"
#include "../lib/hw.h"

struct TCBS {
    TCB* tcb;
    time_t time;
    TCBS* next;

    TCBS(TCB* tcb, time_t time) : tcb(tcb), time(time) , next(nullptr) {};
};

class SleepingList {
private:
    static TCBS* head;
    static TCBS* tail;
public:
    static void addSorted(TCBS* tcbs);
    static TCB* removeFirst();
    static void updateSleeping();
};


#endif //PROJECT_BASE_V1_1_SLEEPINGLIST_H
