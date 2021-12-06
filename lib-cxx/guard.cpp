typedef int __guard;

#define _GUARD_PENDING_BIT 0x1
#define _GUARD_BIT 0x100

extern "C"
int __cxa_guard_acquire(__guard *g) {
    // Single thread applications do not need to use locks
    // Only check for recursive init is requiered
    int *gi = (int *) (void *) g;
    if (*gi == 0) {
      *gi = _GUARD_PENDING_BIT;
      return 1;
    }

    // stub
    return 1;
}
extern "C"
void __cxa_guard_release(__guard *g) {
    // No need to use atomics, and no other threads to wake.
    int *gi = (int *) (void *) g;
    *gi = _GUARD_BIT;
}
extern "C"
void __cxa_guard_abort(__guard *g) {
    // No need to use atomics, and no other threads to wake.
    int *gi = (int *) (void *) g;
    *gi = 0;
}