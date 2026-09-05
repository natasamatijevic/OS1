#include "../h/TCB.hpp"
#include "../h/Scheduler.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/SleepingList.hpp"

TCB* TCB::running = nullptr;
uint64 TCB::timeCnt = 0;

void TCB::yield() {
    __asm__ volatile("mv a0, %0" : : "r"(THREAD_DISPATCH));
    __asm__ __volatile__("ecall");
}

void TCB::wrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setState(FINISHED);
    TCB::yield();
}

void TCB::dispatch() {

    TCB* old = running;
    if (old->isReady()) {
        Scheduler::put(old);
    }
    running = Scheduler::get();
    if (running == nullptr && !old->isFinished()) {
        running = old;
    }

    if (old != running) {
        TCB::contextSwitch(&old->context, &running->context);
    }
}

int TCB::createThread(TCB** handle , Body body, void* arg, uint64* stack) {
    *handle = new TCB(body, arg, DEFAULT_TIME_SLICE, stack);
    if (!*handle) return -1;
    return 0;
}

int TCB::createMain() {
    TCB* mainTCB = new TCB(nullptr, nullptr, DEFAULT_TIME_SLICE, nullptr);
    mainTCB->setState(READY);
    TCB::running = mainTCB;
    return 0;
}

TCB::TCB(Body body, void *arg, uint64 timeSlice, uint64 *stack) :
    body(body), arg(arg), stack(stack), timeSlice(timeSlice),
    state(READY),
    context({(uint64) &wrapper,
        stack != nullptr ? (uint64)((char*)stack) : 0}) {

    if (body != nullptr ) {
        Scheduler::put(this);
    }
}

TCB::~TCB() {
    if (stack) {
        MemoryAllocator::getInstance().mem_free(stack);
    }
}

int TCB::sleep(time_t time) {
    if (time == 0) return -1;

    TCB* goToSleep = running;
    goToSleep->setState(SLEEPING);
    TCBS* tcbs = new TCBS(goToSleep, time);
    SleepingList::addSorted(tcbs);
    dispatch();
    return 0;
}

