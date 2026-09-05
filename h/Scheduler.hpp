#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP

#include "../h/List.hpp"
#include "../h/TCB.hpp"

class Scheduler {
private:
    static List<TCB> readyQueue;
public:
    static TCB* get();
    static void put(TCB* tcb);
};


#endif //PROJECT_BASE_V1_1_SCHEDULER_HPP
