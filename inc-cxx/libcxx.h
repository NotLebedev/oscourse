/* Main public header file for our user-land support library,
 * whose code lives in the lib directory.
 * This library is roughly our OS's version of a standard C library,
 * and is intended to be linked into all user-mode applications
 * (NOT the kernel or boot loader). */

#ifndef JOS_INC_LIB_H
#define JOS_INC_LIB_H 1

#include <inc-cxx/types.h>
#include <inc-cxx/env.h>
#include <inc-cxx/syscall.h>

#ifdef SANITIZE_USER_SHADOW_BASE
/* asan unpoison routine used for whitelisting regions. */
extern "C" void platform_asan_unpoison(void *, size_t);
extern "C" void platform_asan_poison(void *, size_t);
/* non-sanitized memcpy and memset allow us to access "invalid" areas for extra poisoning. */
extern "C" void *__nosan_memset(void *, int, size_t);
extern "C" void *__nosan_memcpy(void *, const void *src, size_t);
#endif

#define USED(x) (void)(x)

/* main user program */
int main(int argc, char **argv);

/* libmain.c or entry.S */
extern const char *binaryname;
extern const volatile int vsys[];
extern const volatile struct Env *thisenv;
extern const volatile struct Env envs[NENV];

/* exit.c */
extern "C" void exit(void);

/* pgfault.c */
typedef bool(pf_handler_t)(struct UTrapframe *utf);
extern "C" int add_pgfault_handler(pf_handler_t handler);
extern "C" void remove_pgfault_handler(pf_handler_t handler);

/* readline.c */
extern "C" char *readline(const char *buf);

/* syscall.c */
#define CURENVID 0

/* sys_alloc_region() specific flags */
#define ALLOC_ZERO 0x100000 /* Allocate memory filled with 0x00 */
#define ALLOC_ONE  0x200000 /* Allocate memory filled with 0xFF */

/* Memory protection flags & attributes
 * NOTE These should be in-sync with kern/pmap.h
 * TODO Create dedicated header for them */
#define PROT_X       0x1 /* Executable */
#define PROT_W       0x2 /* Writable */
#define PROT_R       0x4 /* Readable (mostly ignored) */
#define PROT_RW      (PROT_R | PROT_W)
#define PROT_WC      0x8 /* Write-combining */
#define PROT_CD      0x18 /* Cache disable */
#define PROT_SHARE   0x40 /* Shared copy flag */
#define PROT_LAZY    0x80 /* Copy-on-Write flag */
#define PROT_COMBINE 0x100 /* Combine old and new priviliges */
#define PROT_AVAIL   0xA00 /* Free-to-use flags, available for applications */
/* (mapped directly to page table unused flags) */
#define PROT_ALL 0x05F /* NOTE This definition differs from kernel definition */

extern "C" void sys_cputs(const char *string, size_t len);
extern "C" int sys_cgetc(void);
extern "C" envid_t sys_getenvid(void);
extern "C" int sys_env_destroy(envid_t);
extern "C" void sys_yield(void);
extern "C" int sys_region_refs(void *va, size_t size);
extern "C" int sys_region_refs2(void *va, size_t size, void *va2, size_t size2);
extern "C" {
    static envid_t sys_exofork(void);
}
extern "C" int sys_env_set_status(envid_t env, int status);
extern "C" int sys_env_set_trapframe(envid_t env, struct Trapframe *tf);
extern "C" int sys_env_set_pgfault_upcall(envid_t env, void *upcall);
extern "C" int sys_alloc_region(envid_t env, void *pg, size_t size, int perm);
extern "C" int sys_map_region(envid_t src_env, void *src_pg,
                   envid_t dst_env, void *dst_pg, size_t size, int perm);
extern "C" int sys_unmap_region(envid_t env, void *pg, size_t size);
extern "C" int sys_ipc_try_send(envid_t to_env, uint64_t value, void *pg, size_t size, int perm);
extern "C" int sys_ipc_recv(void *rcv_pg, size_t size);
extern "C" int sys_gettime(void);

extern "C" int vsys_gettime(void);

/* This must be inlined. Exercise for reader: why? */
extern "C" {
    static inline envid_t __attribute__((always_inline))
    sys_exofork(void) {
        envid_t ret;
        asm volatile("int %2"
                     : "=a"(ret)
                     : "a"(SYS_exofork), "i"(T_SYSCALL));
        return ret;
    }
}

/* ipc.c */
extern "C" void ipc_send(envid_t to_env, uint32_t value, void *pg, size_t size, int perm);
extern "C" int32_t ipc_recv(envid_t *from_env_store, void *pg, size_t *psize, int *perm_store);
extern "C" envid_t ipc_find_env(enum EnvType type);

/* fork.c */
extern "C" envid_t fork(void);
extern "C" envid_t sfork(void);

/* uvpt.c */
extern "C" int foreach_shared_region(int (*fun)(void *start, void *end, void *arg), void *arg);
extern "C" pte_t get_uvpt_entry(void *addr);
extern "C" int get_prot(void *va);
extern "C" bool is_page_dirty(void *va);
extern "C" bool is_page_present(void *va);

/* fd.c */
extern "C" int close(int fd);
extern "C" ssize_t read(int fd, void *buf, size_t nbytes);
extern "C" ssize_t write(int fd, const void *buf, size_t nbytes);
extern "C" int seek(int fd, off_t offset);
extern "C" void close_all(void);
extern "C" ssize_t readn(int fd, void *buf, size_t nbytes);
extern "C" int dup(int oldfd, int newfd);
extern "C" int fstat(int fd, struct Stat *statbuf);
extern "C" int stat(const char *path, struct Stat *statbuf);

/* file.c */
extern "C" int open(const char *path, int mode);
extern "C" int ftruncate(int fd, off_t size);
extern "C" int remove(const char *path);
extern "C" int sync(void);

/* spawn.c */
extern "C" envid_t spawn(const char *program, const char **argv);
extern "C" envid_t spawnl(const char *program, const char *arg0, ...);

/* console.c */
extern "C" void cputchar(int c);
extern "C" int getchar(void);
extern "C" int iscons(int fd);
extern "C" int opencons(void);

/* pipe.c */
extern "C" int pipe(int pipefds[2]);
extern "C" int pipeisclosed(int pipefd);

/* wait.c */
extern "C" void wait(envid_t env);

/* File open modes */
#define O_RDONLY  0x0000 /* open for reading only */
#define O_WRONLY  0x0001 /* open for writing only */
#define O_RDWR    0x0002 /* open for reading and writing */
#define O_ACCMODE 0x0003 /* mask for above modes */

#define O_CREAT 0x0100 /* create if nonexistent */
#define O_TRUNC 0x0200 /* truncate to zero length */
#define O_EXCL  0x0400 /* error if already exists */
#define O_MKDIR 0x0800 /* create directory, not regular file */

#ifdef JOS_PROG
extern "C" extern void (*volatile sys_exit)(void);
extern "C" extern void (*volatile sys_yield)(void);
#endif

#ifndef debug
#define debug 0
#endif

#endif /* !JOS_INC_LIB_H */
