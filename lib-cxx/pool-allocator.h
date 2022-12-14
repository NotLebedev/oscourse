#ifndef MEMORYMANAGER_POOLALLOCATOR_H
#define MEMORYMANAGER_POOLALLOCATOR_H

#include <stddef.h>

class PoolAllocator {
public:
    virtual ~PoolAllocator() {};
    virtual void *alloc(size_t size, size_t align) = 0;
    virtual void free(void *addr) = 0;
    virtual size_t getChunkSize(void *addr) = 0;
    virtual void refresh(void *addr, size_t new_size) = 0;
};

PoolAllocator *get();
void abortFree();

#endif //MEMORYMANAGER_POOLALLOCATOR_H
