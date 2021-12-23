#include <stdint.h>
#include <inc/lib.h>
#include <inc-cxx/stdlib.h>
#include "pool-allocator.h"

#define IS_ALIGNED(ADDR, ALIGN) (align == 0 ? true : \
    ((uintptr_t)(const void *)(ADDR)) % (ALIGN) == 0)

#define EXTRA_SMALL_CHUNK 64
#define SMALL_CHUNK (EXTRA_SMALL_CHUNK * 8)
#define MEDIUM_CHUNK (SMALL_CHUNK * 8)
#define LARGE_CHUNK (MEDIUM_CHUNK * 8)
#define EXTRA_LARGE_CHUNK (LARGE_CHUNK * 8)

#define CHUNK_FIXED_SIZE EXTRA_LARGE_CHUNK * 8

static size_t poolProperties[][2] = {
        {EXTRA_SMALL_CHUNK, CHUNK_FIXED_SIZE / EXTRA_SMALL_CHUNK},
        {SMALL_CHUNK, CHUNK_FIXED_SIZE / SMALL_CHUNK},
        {SMALL_CHUNK, CHUNK_FIXED_SIZE / SMALL_CHUNK},
        {SMALL_CHUNK, CHUNK_FIXED_SIZE / SMALL_CHUNK},
        {MEDIUM_CHUNK, CHUNK_FIXED_SIZE / MEDIUM_CHUNK},
        {LARGE_CHUNK, CHUNK_FIXED_SIZE / LARGE_CHUNK},
        {EXTRA_LARGE_CHUNK, CHUNK_FIXED_SIZE / EXTRA_LARGE_CHUNK}
};

#define POOL_PROPERTIES_SIZE (sizeof (poolProperties) / sizeof (poolProperties[0]))

extern uint8_t __bss_end;

struct NoAsanChunk {
    NoAsanChunk *next;
};

class Pool {
public:
    /**
     * Get chunk from this pool of it's size with alignment align
     * @param align requested alignment for chunk
     * @return pointer to chunk or null if alignment cannot be satisfied or pool empty
     */
    virtual void *getChunk(size_t align) = 0;

    /**
     * Try returning chunk to this pool
     * @return true if chunk was from this pool and returned successfully, false otherwise
     */
    virtual bool returnChunk(void *addr) = 0;

    /**
     * @param addr addr to check
     * @return true if addr is from this pool, false otherwise
     */
    virtual bool fromThisPool(void *addr) = 0;
};

class PoolNoAsanImpl : Pool {
public:
    void init(size_t chunkSize, size_t chunkCount) {
        chunkSize_ = chunkSize;
        chunkCount_ = chunkCount;
    }
    ~PoolNoAsanImpl();

    void *getChunk(size_t align) override;

    bool returnChunk(void *addr) override;

    bool fromThisPool(void *addr) override;

private:
    size_t chunkSize_;
    size_t chunkCount_;
    void init_lazy();
    bool poolReady = false;
    NoAsanChunk *chunk;
    uint8_t *pool;
};

void *getMemory() {
    static int chunkIdx = 0;
    void *pool_ptr = ROUNDUP(&__bss_end + PAGE_SIZE, CHUNK_FIXED_SIZE) +
        chunkIdx * CHUNK_FIXED_SIZE;
    sys_alloc_region(CURENVID, pool_ptr, CHUNK_FIXED_SIZE, PROT_RW);
    return pool_ptr;
}

void PoolNoAsanImpl::init_lazy() {
    poolReady = true;
    pool = (uint8_t *) getMemory();
    for (size_t i = 0; i < chunkCount_; i++)
        ((NoAsanChunk *)(pool + chunkSize_ * i))->next = ((i != chunkCount_ - 1) ?
                (NoAsanChunk *)(pool + chunkSize_ * (i + 1)) : nullptr);
    chunk = (NoAsanChunk *)pool;
}

void *PoolNoAsanImpl::getChunk(size_t align) {
    if (!poolReady)
        init_lazy();
    if (chunk == nullptr)
        return nullptr;

    NoAsanChunk *prev = nullptr;
    NoAsanChunk *cur = chunk;
    while (cur != nullptr && !IS_ALIGNED(cur, align)) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == nullptr)
        return nullptr;

    if (prev == nullptr)
        chunk = cur->next;
    else
        prev->next = cur->next;

    return cur;
}

bool PoolNoAsanImpl::returnChunk(void *addr) {
    if (!poolReady || addr < pool || addr > pool + chunkSize_ * chunkCount_ ||
        ((uint8_t *)addr - pool) % chunkSize_ != 0)
        return false;

    auto *ret = static_cast<NoAsanChunk *>(addr);
    ret->next = chunk;
    chunk = ret;
    return true;
}

PoolNoAsanImpl::~PoolNoAsanImpl() {
    sys_unmap_region(CURENVID, pool, CHUNK_FIXED_SIZE);
}

bool PoolNoAsanImpl::fromThisPool(void *addr) {
    return addr >= pool && addr < pool + chunkSize_ * chunkCount_ && ((uint8_t *)addr - pool) % chunkSize_ == 0;
}

class PoolAllocatorNoAsanImpl : public PoolAllocator {
public:
    PoolAllocatorNoAsanImpl();
    ~PoolAllocatorNoAsanImpl() = default;

    void *alloc(size_t size, size_t align) override;

    void free(void *addr) override;

    size_t getChunkSize(void *addr) override;
private:
    PoolNoAsanImpl pools[POOL_PROPERTIES_SIZE];
};

void *PoolAllocatorNoAsanImpl::alloc(size_t size, size_t align) {
    // Allocation priority logic:
    // go through all possible pools listed in pool properties
    // try finding first where size fits and allocate there
    // if it fails continue, return allocation result otherwise
    for (size_t i = 0; i < POOL_PROPERTIES_SIZE; i++)
        if (size <= poolProperties[i][0]) {
            void *res = pools[i].getChunk(align);
            if (res != nullptr)
                return res;
        }
    return nullptr;
}

void PoolAllocatorNoAsanImpl::free(void *addr) {
    for (auto & pool : pools)
        if (pool.returnChunk(addr))
            return;
    // printf("Free of address that was not previously returned by malloc");
    std::abort();
}

PoolAllocatorNoAsanImpl::PoolAllocatorNoAsanImpl() {
    for (size_t i = 0; i < POOL_PROPERTIES_SIZE; i++)
        pools[i].init(poolProperties[i][0], poolProperties[i][1]);
}


static PoolAllocatorNoAsanImpl impl{};
PoolAllocator *get() {
    // Warning, not thread-safe at all. Luckily there are no threads in this
    // version of JOS, so well, whatever ¯\_(ツ)_/¯
    return &impl;
}

size_t PoolAllocatorNoAsanImpl::getChunkSize(void *addr) {
    for (size_t i = 0; i < POOL_PROPERTIES_SIZE; i++)
        if (pools[i].fromThisPool(addr))
            return poolProperties[i][0];

    return 0;
}
