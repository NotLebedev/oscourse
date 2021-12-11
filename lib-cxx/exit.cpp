#include <inc-cxx/libcxx.h>
#include "atexit.h"

#include <inc-cxx/stdio.h>
extern "C"
void exit(void) {
    __cxa_finalize(nullptr);

#ifdef COMP_GXX
    /* Call all global destructors */
    extern void (*__dtors_start)(), (*__dtors_end)();
    void (**dtor)() = &__dtors_start;
    while (dtor < &__dtors_end) 
        (*dtor++)();
#endif

    close_all();

    sys_env_destroy(0);
}