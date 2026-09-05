#include "../h/MemoryAllocator.hpp"
#include "../lib/hw.h"

MemoryAllocator::MemoryAllocator() {
    head = (FREEMEM*) HEAP_START_ADDR;
    head->size = ((size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR) / MEM_BLOCK_SIZE;
    head->next = nullptr;
}
MemoryAllocator& MemoryAllocator::getInstance() {
    static MemoryAllocator instance;
    return instance;
}

void MemoryAllocator::trySplit(FREEMEM *node, size_t size) {
    if (node->size == size) return;
    FREEMEM* newNode = (FREEMEM*) ((size_t)node + size*MEM_BLOCK_SIZE);
    newNode->size = node->size - size;
    newNode->next = node->next;
    node->next = newNode;
    node->size = size;
    return;
}

void MemoryAllocator::removeFromList(FREEMEM *prev, FREEMEM *curr) {
    if (prev == nullptr) {
        head = curr->next;
        curr->next = nullptr;
        return;
    }
    prev->next = curr->next;
    curr->next = nullptr;

}

void* MemoryAllocator::mem_alloc(size_t size) {
    if (size == 0) return nullptr;
    FREEMEM* curr = head;
    FREEMEM* prev = nullptr;
    size++;
    while(curr != nullptr) {
        if (curr->size >= size) {
            trySplit(curr, size);
            removeFromList(prev, curr);
            return (void*) ((size_t)curr + MEM_BLOCK_SIZE);
        }
        prev = curr;
        curr = curr->next;
    }
    return nullptr;
}

void MemoryAllocator::tryToJoin(FREEMEM *node1, FREEMEM *node2) {
    if (node1 != nullptr && node2 != nullptr && ((size_t)node1 + node1->size * MEM_BLOCK_SIZE == (size_t)node2)) {
        node1->size += node2->size;
        node1->next = node2->next;
    }
}

void MemoryAllocator::addToList(FREEMEM *node) {
    if (head == nullptr) {
        head = node;
        return;
    }
    FREEMEM* prev = nullptr;
    FREEMEM* curr = head;

    while (curr != nullptr && curr < node) {
        prev = curr;
        curr = curr->next;
    }

    if (prev != nullptr) {
        prev->next = node;
    } else {
        head = node;
    }
    node->next = curr;

    tryToJoin(node, curr);
    tryToJoin(prev, node);
}

int MemoryAllocator::mem_free(void *ptr) {
    if (ptr == nullptr) return -1;

    ptr = (void*) ((size_t)ptr - MEM_BLOCK_SIZE);
    if (ptr < HEAP_START_ADDR || ptr >= HEAP_END_ADDR) {
        return -1;
    }

    FREEMEM* freeNode = (FREEMEM*) ptr;
    addToList(freeNode);
    return 0;

}


