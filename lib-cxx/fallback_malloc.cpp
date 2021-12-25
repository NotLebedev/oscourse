//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "fallback_malloc.h"


#include <inc-cxx/stdlib.h> // for malloc, calloc, free
#include <inc/string.h> // for memset
#include <inc-cxx/new> // for std::__libcpp_aligned_{alloc,free}

//  A small, simple heap manager based (loosely) on
//  the startup heap manager from FreeBSD, optimized for space.
//
//  Manages a fixed-size memory pool, supports malloc and free only.
//  No support for realloc.
//
//  Allocates chunks in multiples of four bytes, with a four byte header
//  for each chunk. The overhead of each chunk is kept low by keeping pointers
//  as two byte offsets within the heap, rather than (4 or 8 byte) pointers.

namespace {

// When POSIX threads are not available, make the mutex operations a nop
static void* heap_mutex = 0;

class mutexor {
public:

  mutexor(void*) {}
  ~mutexor() {}
private:
  mutexor(const mutexor& rhs);
  mutexor& operator=(const mutexor& rhs);
};

static const size_t HEAP_SIZE = 512;
char heap[HEAP_SIZE] __attribute__((aligned));

typedef unsigned short heap_offset;
typedef unsigned short heap_size;

struct heap_node {
  heap_offset next_node; // offset into heap
  heap_size len;         // size in units of "sizeof(heap_node)"
};

static const heap_node* list_end =
    (heap_node*)(&heap[HEAP_SIZE]); // one past the end of the heap
static heap_node* freelist = NULL;

heap_node* node_from_offset(const heap_offset offset) {
  return (heap_node*)(heap + (offset * sizeof(heap_node)));
}

heap_offset offset_from_node(const heap_node* ptr) {
  return static_cast<heap_offset>(
      static_cast<size_t>(reinterpret_cast<const char*>(ptr) - heap) /
      sizeof(heap_node));
}

void init_heap() {
  freelist = (heap_node*)heap;
  freelist->next_node = offset_from_node(list_end);
  freelist->len = HEAP_SIZE / sizeof(heap_node);
}

//  How big a chunk we allocate
size_t alloc_size(size_t len) {
  return (len + sizeof(heap_node) - 1) / sizeof(heap_node) + 1;
}

bool is_fallback_ptr(void* ptr) {
  return ptr >= heap && ptr < (heap + HEAP_SIZE);
}

void* fallback_malloc(size_t len) {
  heap_node *p, *prev;
  const size_t nelems = alloc_size(len);
  mutexor mtx(&heap_mutex);

  if (NULL == freelist)
    init_heap();

  //  Walk the free list, looking for a "big enough" chunk
  for (p = freelist, prev = 0; p && p != list_end;
       prev = p, p = node_from_offset(p->next_node)) {

    if (p->len > nelems) { //  chunk is larger, shorten, and return the tail
      heap_node* q;

      p->len = static_cast<heap_size>(p->len - nelems);
      q = p + p->len;
      q->next_node = 0;
      q->len = static_cast<heap_size>(nelems);
      return (void*)(q + 1);
    }

    if (p->len == nelems) { // exact size match
      if (prev == 0)
        freelist = node_from_offset(p->next_node);
      else
        prev->next_node = p->next_node;
      p->next_node = 0;
      return (void*)(p + 1);
    }
  }
  return NULL; // couldn't find a spot big enough
}

//  Return the start of the next block
heap_node* after(struct heap_node* p) { return p + p->len; }

void fallback_free(void* ptr) {
  struct heap_node* cp = ((struct heap_node*)ptr) - 1; // retrieve the chunk
  struct heap_node *p, *prev;

  mutexor mtx(&heap_mutex);

  for (p = freelist, prev = 0; p && p != list_end;
       prev = p, p = node_from_offset(p->next_node)) {
    if (after(p) == cp) {
      p->len = static_cast<heap_size>(
          p->len + cp->len); // make the free heap_node larger
      return;
    } else if (after(cp) == p) { // there's a free heap_node right after
      cp->len = static_cast<heap_size>(cp->len + p->len);
      if (prev == 0) {
        freelist = cp;
        cp->next_node = p->next_node;
      } else
        prev->next_node = offset_from_node(cp);
      return;
    }
  }
//  Nothing to merge with, add it to the start of the free list
  cp->next_node = offset_from_node(freelist);
  freelist = cp;
}
} // end unnamed namespace

namespace __cxxabiv1 {

struct __attribute__((aligned)) __aligned_type {};

void* __aligned_malloc_with_fallback(size_t size) {
  if (size == 0)
    size = 1;
  if (void* dest = aligned_alloc(__alignof(__aligned_type), size))
    return dest;
  return fallback_malloc(size);
}

void* __calloc_with_fallback(size_t count, size_t size) {
  void* ptr = calloc(count, size);
  if (NULL != ptr)
    return ptr;
  // if calloc fails, fall back to emergency stash
  ptr = fallback_malloc(size * count);
  if (NULL != ptr)
    memset(ptr, 0, size * count);
  return ptr;
}

void __aligned_free_with_fallback(void* ptr) {
  if (is_fallback_ptr(ptr))
    fallback_free(ptr);
  else {
    free(ptr);
  }
}

void __free_with_fallback(void* ptr) {
  if (is_fallback_ptr(ptr))
    fallback_free(ptr);
  else
    free(ptr);
}

} // namespace __cxxabiv1
