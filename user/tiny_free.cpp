/*
 * Institute for System Programming of the Russian Academy of Sciences
 * Copyright (C) 2018 ISPRAS
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, Version 3.
 *
 * This program is distributed in the hope # that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License version 3 for more details.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdalign.h>

#define printf(...) do { } while(0)

static void *test_type(const char *name, size_t size, size_t mask, size_t elemnum) {
    (void)name;
    const char *allocators[] = {
        "malloc",
        "calloc",
        "realloc",
        "aligned_alloc"
    };

    for (size_t it = 0; it < sizeof(allocators)/sizeof(allocators[0]); it++) {
        printf("Testing %s sizeof(%s) = %zu: ", allocators[it], name, size);
        uint8_t *p = NULL;
        bool inited = false;

        if      (it == 0) { p = (uint8_t *)malloc (elemnum*size); }
        else if (it == 1) { p = (uint8_t *)calloc (elemnum, size); inited = true; }
        else if (it == 2) { p = (uint8_t *)realloc(NULL, elemnum*size); }
        else if (it == 3) { p = (uint8_t *)aligned_alloc(16, elemnum*size); }

        printf("(%p) ", p);
        if (p == NULL) {
            (printf)("ALLOC FAIL\n");
            abort();
        }
        if (((uintptr_t)p & mask) != 0) {
            (printf)("ALIGN FAIL\n");
            abort();
        }
        if (inited) {
            // MSAN_CHECK(p, size);
            for (size_t i = 0; i < size; i++) {
                if (p[i] != 0) {
                    (printf)("INIT FAIL at %zu\n", i);
                    abort();
                }
            }
        } else {
            // Will invoke ASAN.
            for (size_t i = 0; i < size; i++) {
                p[i] = (uint8_t)(i % 256);
            }
        }

        if (it + 1 == sizeof(allocators)/sizeof(allocators[0])) {
            printf("OK\n");
            return p;
        }

        free(p);
        printf("OK\n");
    }

    return NULL;
}

static void *test_align(const char *name, size_t size, size_t align) {
    (void)name;

    // Should properly align.
    printf("Testing aligned %s: ", name);
    uint8_t *p = (uint8_t *) aligned_alloc (align, size);
    printf("(%p) ", p);
    if (((uintptr_t)p & (align-1)) != 0) {
        (printf)("ALIGN FAIL\n");
        abort();
    }
    // Will invoke ASAN.
    for (size_t i = 0; i < size; i++) {
        p[i] = (uint8_t)(i % 256);
    }
    printf("OK\n");

    return p;
}

static int test_main_runner(void)
{
    // Can either return NULL or some value. You are allowed to free it.
    printf("Testing 0: ");
    void *p = malloc (0);
    printf("(%p) ", p);
    free(p);
    printf("OK\n");

    printf("Testing large: ");
    p = malloc (262144);
    printf("(%p) ", p);
    free(p);
    printf("OK\n");

    // Can either return NULL or some value. You are allowed to free it.
    printf("Testing 0 -> 1: ");
    p = malloc (0);
    printf("(%p) ", p);
    void *np = realloc(p, 1);
    if (np == NULL) {
        (printf)("REALLOC FAIL\n");
        abort();
    }
    *(uint8_t *)np = 1;
    free(np);
    printf("OK\n");

    void *ap1 = test_align("a8", 1, 8);
    void *ap2 = test_align("a16", 1, 16);
    void *ap3 = test_align("a32", 1, 32);
    void *ap4 = test_align("a64", 1, 64);
    void *ap5 = test_align("a128", 1, 128);

    free(ap1);
    free(ap2);
    free(ap3);
    free(ap4);
    free(ap5);

    for (size_t elemnum = 1; elemnum < 500; elemnum *= 7) {
        // Test primitive types
        void *p1 = test_type("uint8_t", sizeof(uint8_t), alignof(uint8_t)-1, elemnum);
        void *p2 = test_type("uint16_t", sizeof(uint16_t), alignof(uint16_t)-1, elemnum);
        void *p3 = test_type("uint32_t", sizeof(uint32_t), alignof(uint32_t)-1, elemnum);
        void *p4 = test_type("uint64_t", sizeof(uint64_t), alignof(uint64_t)-1, elemnum);
        void *p5 = test_type("int8_t", sizeof(int8_t), alignof(int8_t)-1, elemnum);
        void *p6 = test_type("int16_t", sizeof(int16_t), alignof(int16_t)-1, elemnum);
        void *p7 = test_type("int32_t", sizeof(int32_t), alignof(int32_t)-1, elemnum);
        void *p8 = test_type("int64_t", sizeof(int64_t), alignof(int64_t)-1, elemnum);
        void *p9 = test_type("unsigned char", sizeof(unsigned char), alignof(unsigned char)-1, elemnum);
        void *p10 = test_type("unsigned short", sizeof(unsigned short), alignof(unsigned short)-1, elemnum);
        void *p11 = test_type("unsigned int", sizeof(unsigned int), alignof(unsigned int)-1, elemnum);
        void *p12 = test_type("unsigned long", sizeof(unsigned long), alignof(unsigned long)-1, elemnum);
        void *p13 = test_type("unsigned long long", sizeof(unsigned long long), alignof(unsigned long long)-1, elemnum);
        void *p14 = test_type("char", sizeof(char), alignof(char)-1, elemnum);
        void *p15 = test_type("short", sizeof(short), alignof(short)-1, elemnum);
        void *p16 = test_type("int", sizeof(int), alignof(int)-1, elemnum);
        void *p17 = test_type("long", sizeof(long), alignof(long)-1, elemnum);
        void *p18 = test_type("long long", sizeof(long long), alignof(long long)-1, elemnum);
        void *p19 = test_type("size_t", sizeof(size_t), alignof(size_t)-1, elemnum);
        void *p20 = test_type("uintptr_t", sizeof(uintptr_t), alignof(uintptr_t)-1, elemnum);
        void *p21 = test_type("ptrdiff_t", sizeof(ptrdiff_t), alignof(ptrdiff_t)-1, elemnum);
        void *p22 = test_type("intptr_t", sizeof(intptr_t), alignof(intptr_t)-1, elemnum);
        void *p23 = test_type("max_align_t", sizeof(max_align_t), alignof(max_align_t)-1, elemnum);
        // Test that for large types the alignment is at least sizeof(max_align_t).
        void *p24 = test_type("large_type", sizeof(max_align_t) + 13, alignof(max_align_t)-1, elemnum);

        free(p1);
        free(p2);
        free(p3);
        free(p4);
        free(p5);
        free(p6);
        free(p7);
        free(p8);
        free(p9);
        free(p10);
        free(p11);
        free(p12);
        free(p13);
        free(p14);
        free(p15);
        free(p16);
        free(p17);
        free(p18);
        free(p19);
        free(p20);
        free(p21);
        free(p22);
        free(p23);
        free(p24);
    }

    return 0;
}

#undef printf

int main(int argc, char **argv)
{
    printf("Will run basic allocation tests in system\n");

    // Run tests in system
    for (size_t i = 0; i < 100; i++) {
        test_main_runner();
    }

    if (sizeof(void *) == sizeof(uint32_t)) {
        printf("Will check 32-bit overflow\n");
        void *ovf = calloc(0xFF, 0x1010102);
        if (ovf != NULL) {
            printf("Overflow\n");
            abort();
        }
    }

    printf("Will brainwash malloc\n");
    // Brainwash malloc
    for (uint32_t i = 0; i < 0xFFFU; i++) {
        uint8_t *p = (uint8_t *)malloc(i);
        if (p != NULL) {
            for (size_t j = 0; j < i; j++) {
                p[j] = (uint8_t)(j % 256);
            }
            free(p);
        }
    }

    printf("Will brainwash malloc v2\n");
    void *ptr[24];
    for (uint32_t i = 0; i < 24; i++) {
        ptr[i] = malloc(1U << i);
    }

    for (uint32_t i = 0; i < 24; i++) {
        free(ptr[i]);
    }

    // Ensure that realloc(n, 0) frees memory, allocated by malloc.
    // See 0178ab35.
    for (uint32_t i = 0; i < 10; i++) {
        void *p = malloc(1024*256);
        if (p == NULL) {
            printf("Borked free\n");
            abort();
        }
        void *n = realloc(p, 0);
        if (n != NULL) {
           free(n);
        } else {
           free(p);
        }
    }

    printf("All OK\n");

    return 0;
}
