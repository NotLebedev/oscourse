#ifndef JOS_INC_STDLIB_H
#define JOS_INC_STDLIB_H

#include <stddef.h>

namespace std {

extern "C" int atexit(void (*func)(void));
extern "C" void exit(void);
extern "C" void* malloc(size_t size);
extern "C" void* aligned_alloc(size_t alignment, size_t size);
extern "C" void* calloc(size_t num, size_t size);
extern "C" void free(void* ptr);
extern "C" void* realloc(void* ptr, size_t new_size);

extern "C" {
[[noreturn]] void abort() noexcept;
}

}

extern "C" int atexit(void (*func)(void));
extern "C" void exit(void);
extern "C" void* malloc(size_t size);
extern "C" void* aligned_alloc(size_t alignment, size_t size);
extern "C" void* calloc(size_t num, size_t size);
extern "C" void free(void* ptr);
extern "C" void* realloc(void* ptr, size_t new_size);
extern "C" {
[[noreturn]] void abort() noexcept;
}

#endif /* !JOS_INC_STDLIB_H */
