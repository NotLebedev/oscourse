#include "atexit.h"

struct atexit_func_entry_t __atexit_funcs[ATEXIT_MAX_FUNCS];
unsigned int __atexit_func_count = 0;
 
void *__dso_handle = nullptr;
 
extern "C"
int atexit(void (*func)(void)) {
    return __cxa_atexit ((void (*) (void *)) func, nullptr, __dso_handle);
}

extern "C"
int __cxa_atexit(void (*func)(void *), void *objptr, void *dso) {
    if (__atexit_func_count >= ATEXIT_MAX_FUNCS) 
        return -1;

    __atexit_funcs[__atexit_func_count].destructor_func = func;
    __atexit_funcs[__atexit_func_count].obj_ptr = objptr;
    __atexit_funcs[__atexit_func_count].dso_handle = dso;
    __atexit_func_count++;

    return 0;
};

extern "C"
void __cxa_finalize(void *func) {
    unsigned int i = __atexit_func_count;

    if (func == nullptr) {
        while (i--)
            if (__atexit_funcs[i].destructor_func) {
                (*__atexit_funcs[i].destructor_func)(__atexit_funcs[i].obj_ptr);
            }

        return;
    }
 
    while (i--)
        if (__atexit_funcs[i].destructor_func == func) {
            (*__atexit_funcs[i].destructor_func)(__atexit_funcs[i].obj_ptr);
            __atexit_funcs[i].destructor_func = 0;
        }
};