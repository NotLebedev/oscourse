#ifndef JOS_INC_ATEXIT_H
#define JOS_INC_ATEXIT_H

#define ATEXIT_MAX_FUNCS 128

struct atexit_func_entry_t
{
    /*
    * Each member is at least 4 bytes large. Such that each entry is 12bytes.
    * 128 * 12 = 1.5KB exact.
    **/
    void (*destructor_func)(void *);
    void *obj_ptr;
    void *dso_handle;
};
 
extern "C"
int atexit(void (*f)(void));
extern "C"
int __cxa_atexit(void (*f)(void *), void *objptr, void *dso);
extern "C"
void __cxa_finalize(void *f);

extern "C"
void finalize();

#endif // JOS_INC_ATEXIT_H