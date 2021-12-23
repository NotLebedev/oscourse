#include <inc-cxx/stdlib.h>
#include <inc/lib.h>
#include <inc/string.h>

#include "pool-allocator.h"

extern "C" void* malloc(size_t size) {
    PoolAllocator *alloc = get();
    return alloc->alloc(size, alignof(max_align_t));
}

extern "C" void* aligned_alloc(size_t alignment, size_t size) {
    PoolAllocator *alloc = get();
    return alloc->alloc(size, alignment);
}

extern "C" void* calloc(size_t num, size_t size) {
    PoolAllocator *alloc = get();
    void *mem = alloc->alloc(size, alignof(max_align_t));
    if (mem != nullptr)
        memset(mem, 0, size);
    return mem;
}

extern "C" void free(void* ptr) {
    if (ptr == nullptr || ptr == NULL)
        return;
    PoolAllocator *alloc = get();
    alloc->free(ptr);
}

extern "C" void* realloc(void* ptr, size_t new_size) {
    if (ptr == nullptr || ptr == NULL)
        return malloc(new_size);

    PoolAllocator *alloc = get();
    if (new_size == 0) {
        free(ptr);
        return nullptr;
    }
    size_t olChunkSize = alloc->getChunkSize(ptr);
    if (olChunkSize == 0)
        return nullptr;
    if (olChunkSize >= new_size) {
        alloc->refresh(ptr, new_size);
        return ptr;
    }

    void *n = alloc->alloc(new_size, alignof(max_align_t));
    if (n == nullptr)
        return nullptr;

    memcpy(n, ptr, olChunkSize);
    free(ptr);

    return n;
}
