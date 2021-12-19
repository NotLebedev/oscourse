#ifndef JOS_INC_STDIO_H
#define JOS_INC_STDIO_H

#include <inc-cxx/stdarg.h>
#include <stddef.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

/* lib/stdio.c */
extern "C"
void cputchar(int c);
extern "C"
int getchar(void);
extern "C"
int iscons(int fd);

/* lib/printfmt.c */
extern "C"
void printfmt(void (*putch)(int, void *), void *putdat, const char *fmt, ...) __attribute__((format(printf, 3, 4)));
extern "C"
void vprintfmt(void (*putch)(int, void *), void *putdat, const char *fmt, va_list) __attribute__((format(printf, 3, 0)));
extern "C"
int snprintf(char *str, size_t size, const char *fmt, ...) __attribute__((format(printf, 3, 4)));
extern "C"
int vsnprintf(char *str, size_t size, const char *fmt, va_list) __attribute__((format(printf, 3, 0)));

/* lib/printf.c */
extern "C"
int cprintf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
extern "C"
int vcprintf(const char *fmt, va_list) __attribute__((format(printf, 1, 0)));

/* lib/fprintf.c */
extern "C"
int printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
extern "C"
int fprintf(int fd, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
extern "C"
int vfprintf(int fd, const char *fmt, va_list);

/* lib/readline.c */
extern "C"
char *readline(const char *prompt);

#endif /* !JOS_INC_STDIO_H */
