/* Main public header file for our user-land support library,
 * whose code lives in the lib directory.
 * This library is roughly our OS's version of a standard C library,
 * and is intended to be linked into all user-mode applications
 * (NOT the kernel or boot loader). */

#ifndef JOS_INC_LIB_H
#define JOS_INC_LIB_H 1

#include <inc/types.h>
#include <inc/env.h>

int main(int, char **);

extern "C" void exit(void);

extern const char *binaryname;
extern const volatile int vsys[];
extern const volatile struct Env *thisenv;
extern const volatile struct Env envs[NENV];

extern "C" envid_t sys_getenvid(void);

extern "C" int sys_env_destroy(envid_t);

extern "C" void close_all(void);

#endif /* !JOS_INC_LIB_H */
