#include "../h/SleepingList.hpp"
#include "../h/Scheduler.hpp"

TCBS* SleepingList::head = nullptr;
TCBS* SleepingList::tail = nullptr;

void SleepingList::addSorted(TCBS* tcbs) {
    if (tcbs == nullptr) {return;}

    if (head == nullptr) {
        head = tail = tcbs;
    }
    else {
        TCBS* curr = head;
        TCBS* prev = nullptr;

        while (curr != nullptr && tcbs->time >= curr->time) {
            prev = curr;
            curr = curr->next;
        }

        if (prev == nullptr) {
            tcbs->next = head;
            head = tcbs;
        }
        else {
            prev->next = tcbs;
            tcbs->next = curr;

            if (curr == nullptr) {
                tail = tcbs;
            }
        }
    }
}

TCB* SleepingList::removeFirst() {
    if (head == nullptr) return nullptr;

    TCBS* temp = head;
    TCB* data = temp->tcb;

    head = head->next;
    if (!head) {
        tail = nullptr;
    }
    delete temp;
    return data;
}

void SleepingList::updateSleeping() {
    TCBS* curr = head;
    while (curr != nullptr) {
        if (curr->time > 0) {
            curr->time--;
        }
        curr = curr->next;
    }

    while (head != nullptr && head->time == 0) {
        TCB* woken = removeFirst();
        if (woken != nullptr) {
            woken->setState(TCB::READY);
            Scheduler::put(woken);
        }
    }
}
