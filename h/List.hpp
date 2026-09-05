#ifndef PROJECT_BASE_V1_1_LIST_H
#define PROJECT_BASE_V1_1_LIST_H

template<typename  T>
class List {
private:
    struct Elem {
        T* data;
        Elem* next;

        Elem(T* data, Elem* next = nullptr) : data(data), next(next) {};
    };

    Elem* head;
    Elem* tail;

public:
    List() : head(nullptr), tail(nullptr) {};

    List(const List&) = delete;
    List& operator=(const List&) = delete;

    void addLast(T* data) {
        if (data == nullptr) return;

        Elem* newElem = new Elem(data);
        if (!head) {
            head = tail = newElem;
        }
        else {
            tail->next = newElem;
            tail = newElem;
        }
    }

    void addFirst(T* data) {
        if (data == nullptr) return;

        Elem* newElem = new Elem(data);
        if (!head) {
            head = tail = newElem;
        }
        else {
            newElem->next = head;
            head = newElem;
        }
    }

    T* removeFirst() {
        if (head == nullptr) return nullptr;

        Elem* temp = head;
        T* data = temp->data;

        head = head->next;
        if (!head) {
            tail = nullptr;
        }
        delete temp;
        return data;
    }

    T* removeLast() {
        if (tail == nullptr) return nullptr;
        Elem* temp = tail;
        T* data = temp->data;

        Elem *prev = nullptr;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next) {
            prev = curr;
        }

        if (prev) {
            prev->next = nullptr;
            tail = prev;
        }
        else {
            head = tail = nullptr;
        }

        delete temp;
        return data;
    }

    T* peekFirst() {
        if (head == nullptr) return nullptr;
        return head->data;
    }

    T* peekLast() {
        if (tail == nullptr) return nullptr;
        return tail->data;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    int length() {
        Elem* curr = head;
        int cnt = 0;
        while (curr) {
            cnt++;
            curr = curr->next;
        }
        return cnt;
    }
};

#endif //PROJECT_BASE_V1_1_LIST_H
