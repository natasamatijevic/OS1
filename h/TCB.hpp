#ifndef PROJECT_BASE_V1_1_TCB_HPP
#define PROJECT_BASE_V1_1_TCB_HPP

#include "../lib/hw.h"
#include "../h/syscall_c.hpp"

class TCB {
public:
    using Body = void (*)(void*);
    uint64 static timeCnt;

    enum State {
        READY,
        WAITING,
        SLEEPING,
        FINISHED
    };

    static int createThread(TCB **handle, Body body, void *arg, uint64 *stack);
    TCB(Body body, void *arg, uint64 timeSlice, uint64 *stack);
    ~TCB();

    static int createMain();

    State getState() const { return state; }
    void setState(State s) { state = s; }
    bool isFinished() const { return state == FINISHED; }
    bool isSleeping() const { return state == SLEEPING; }
    bool isReady() const { return state == READY; }
    bool isWaiting() const { return state == WAITING; }
    uint64 getTimeSlice() const { return timeSlice; }

    static TCB* getRunning() { return running; }
    static void setRunning(TCB* tcb) { running = tcb; }

    static void yield();
    static void dispatch();
    static int exit();

    static int sleep(time_t time);

    friend class Riscv;

private:
    static TCB* running;

    Body body;
    void* arg;
    uint64* stack;
    uint64 timeSlice;
    State state;

    struct Context {
        uint64 ra;
        uint64 sp;
    };
    Context context;

    static void wrapper();
    static void contextSwitch(Context* oldContext, Context* newContext);


};


#endif //PROJECT_BASE_V1_1_TCB_HPP
