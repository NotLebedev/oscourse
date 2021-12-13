#ifndef JOS_INC_STRING_H
#define JOS_INC_STRING_H

#include <inc-cxx/types.h>
namespace std {
extern "C" size_t strlen(const char *s);
extern "C" size_t strnlen(const char *s, size_t size);
extern "C" char *strcpy(char *dst, const char *src);
extern "C" char *strncpy(char *dst, const char *src, size_t size);
extern "C" char *strcat(char *dst, const char *src);
extern "C" size_t strlcpy(char *dst, const char *src, size_t size);
extern "C" size_t strlcat(char *dst, const char *src, size_t maxlen);
extern "C" int strcmp(const char *s1, const char *s2);
extern "C" int strncmp(const char *s1, const char *s2, size_t size);
extern "C" char *strchr(const char *s, int c);
extern "C" char *strfind(const char *s, int c);

extern "C" void *memset(void *dst, int c, size_t len);
extern "C" void *memcpy(void *dst, const void *src, size_t len);
extern "C" void *memmove(void *dst, const void *src, size_t len);
extern "C" int memcmp(const void *s1, const void *s2, size_t len);
extern "C" void *memfind(const void *s, int c, size_t len);

extern "C" long strtol(const char *s, char **endptr, int base);
};

extern "C" size_t strlen(const char *s);
extern "C" size_t strnlen(const char *s, size_t size);
extern "C" char *strcpy(char *dst, const char *src);
extern "C" char *strncpy(char *dst, const char *src, size_t size);
extern "C" char *strcat(char *dst, const char *src);
extern "C" size_t strlcpy(char *dst, const char *src, size_t size);
extern "C" size_t strlcat(char *dst, const char *src, size_t maxlen);
extern "C" int strcmp(const char *s1, const char *s2);
extern "C" int strncmp(const char *s1, const char *s2, size_t size);
extern "C" char *strchr(const char *s, int c);
extern "C" char *strfind(const char *s, int c);

extern "C" void *memset(void *dst, int c, size_t len);
extern "C" void *memcpy(void *dst, const void *src, size_t len);
extern "C" void *memmove(void *dst, const void *src, size_t len);
extern "C" int memcmp(const void *s1, const void *s2, size_t len);
extern "C" void *memfind(const void *s, int c, size_t len);

extern "C" long strtol(const char *s, char **endptr, int base);

#endif /* not JOS_INC_STRING_H */
