#include <inc-cxx/stdlib.h>
#include <stddef.h>
#include <inc-cxx/new>

void * operator new(size_t size) {
    return malloc(size);
}

void* operator new(size_t size, const std::nothrow_t&) noexcept {
    return malloc(size);
}

void* operator new[](size_t size) {
    return ::operator new(size);
}

void* operator new[](size_t size, const std::nothrow_t&) noexcept {
    return ::operator new(size);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&) noexcept {
    ::operator delete(ptr);
}

void
operator delete(void* ptr, size_t) noexcept
{
    ::operator delete(ptr);
}

void
operator delete[] (void* ptr) noexcept
{
    ::operator delete(ptr);
}

void
operator delete[] (void* ptr, const std::nothrow_t&) noexcept
{
    ::operator delete[](ptr);
}

void
operator delete[] (void* ptr, size_t) noexcept
{
    ::operator delete[](ptr);
}

void *operator new(size_t size, std::align_val_t alignment) {
    return aligned_alloc(static_cast<size_t>(alignment), size);
}

void*
operator new(size_t size, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    return aligned_alloc(static_cast<size_t>(alignment), size);
}

void*
operator new[](size_t size, std::align_val_t alignment)
{
    return ::operator new(size, alignment);
}

void*
operator new[](size_t size, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    return ::operator new(size, alignment);
}

void
operator delete(void* ptr, std::align_val_t) noexcept
{
    ::free(ptr);
}

void
operator delete(void* ptr, size_t, std::align_val_t alignment) noexcept
{
    ::operator delete(ptr, alignment);
}

void
operator delete[] (void* ptr, std::align_val_t alignment) noexcept
{
    ::operator delete(ptr, alignment);
}

void
operator delete[] (void* ptr, size_t, std::align_val_t alignment) noexcept
{
    ::operator delete[](ptr, alignment);
}
