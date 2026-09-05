#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {
private:
    MemoryAllocator();
    MemoryAllocator(const MemoryAllocator&) = delete;
    MemoryAllocator& operator= (const MemoryAllocator&) = delete;
    static MemoryAllocator* instance;

    struct FREEMEM {
        size_t size;
        FREEMEM* next;
    };
    FREEMEM* head;

    size_t roundSize(size_t size);
    void trySplit(FREEMEM* node, size_t size);
    void removeFromList(FREEMEM* prev, FREEMEM* curr);
    void addToList(FREEMEM* node);
    void tryToJoin(FREEMEM* node1, FREEMEM* node2);
public:
    static MemoryAllocator& getInstance();
    void* mem_alloc(size_t size);
    int mem_free(void* ptr);

};


#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
