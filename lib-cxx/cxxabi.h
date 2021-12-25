
// ABI Support -*- C++ -*-

// Copyright (C) 2000-2021 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// GCC is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// GCC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

// Written by Nathan Sidwell, Codesourcery LLC, <nathan@codesourcery.com>

/* This file declares the new abi entry points into the runtime. It is not
   normally necessary for user programs to include this header, or use the
   entry points directly. However, this header is available should that be
   needed.
   Some of the entry points are intended for both C and C++, thus this header
   is includable from both C and C++. Though the C++ specific parts are not
   available in C, naturally enough.  */

/** @file cxxabi.h
 *  The header provides an interface to the C++ ABI.
 */

#ifndef _CXXABI_H
#define _CXXABI_H 1

#pragma GCC system_header

#pragma GCC visibility push(default)

#include <inc-cxx/typeinfo>
#include <stddef.h>
#include <stdint.h>

namespace __cxxabiv1
{

    extern "C"  {

    // 2.4.2 Allocating the Exception Object
    extern  void *
    __cxa_allocate_exception(size_t thrown_size) throw();
    extern  void
    __cxa_free_exception(void *thrown_exception) throw();

    // 2.4.3 Throwing the Exception Object
    extern  [[noreturn]] void
    __cxa_throw(void *thrown_exception, std::type_info *tinfo,
                void (*dest)(void *));

    // 2.5.3 Exception Handlers
    extern  void *
    __cxa_get_exception_ptr(void *exceptionObject) throw();
    extern  void *
    __cxa_begin_catch(void *exceptionObject) throw();
    extern  void __cxa_end_catch();
    extern  std::type_info *__cxa_current_exception_type();

    // 2.5.4 Rethrowing Exceptions
    extern  [[noreturn]] void __cxa_rethrow();

    // 2.6 Auxiliary Runtime APIs
    extern  [[noreturn]] void __cxa_bad_cast(void);
    extern  [[noreturn]] void __cxa_bad_typeid(void);
    extern  [[noreturn]] void
    __cxa_throw_bad_array_new_length(void);

    // 3.2.6 Pure Virtual Function API
    extern  [[noreturn]] void __cxa_pure_virtual(void);

    // 3.2.7 Deleted Virtual Function API
    extern  [[noreturn]] void __cxa_deleted_virtual(void);

    // 3.3.2 One-time Construction API
    #if defined(_LIBCXXABI_GUARD_ABI_ARM)
    extern   int __cxa_guard_acquire(uint32_t *);
    extern   void __cxa_guard_release(uint32_t *);
    extern   void __cxa_guard_abort(uint32_t *);
    #else
    extern   int __cxa_guard_acquire(uint64_t *);
    extern   void __cxa_guard_release(uint64_t *);
    extern   void __cxa_guard_abort(uint64_t *);
    #endif

    // 3.3.3 Array Construction and Destruction API
    extern  void *
    __cxa_vec_new(size_t element_count, size_t element_size, size_t padding_size,
                  void (*constructor)(void *), void (*destructor)(void *));

    extern  void *
    __cxa_vec_new2(size_t element_count, size_t element_size, size_t padding_size,
                   void (*constructor)(void *), void (*destructor)(void *),
                   void *(*alloc)(size_t), void (*dealloc)(void *));

    extern  void *
    __cxa_vec_new3(size_t element_count, size_t element_size, size_t padding_size,
                   void (*constructor)(void *), void (*destructor)(void *),
                   void *(*alloc)(size_t), void (*dealloc)(void *, size_t));

    extern  void
    __cxa_vec_ctor(void *array_address, size_t element_count, size_t element_size,
                   void (*constructor)(void *), void (*destructor)(void *));

    extern  void __cxa_vec_dtor(void *array_address,
                                                   size_t element_count,
                                                   size_t element_size,
                                                   void (*destructor)(void *));

    extern  void __cxa_vec_cleanup(void *array_address,
                                                      size_t element_count,
                                                      size_t element_size,
                                                      void (*destructor)(void *));

    extern  void __cxa_vec_delete(void *array_address,
                                                     size_t element_size,
                                                     size_t padding_size,
                                                     void (*destructor)(void *));

    extern  void
    __cxa_vec_delete2(void *array_address, size_t element_size, size_t padding_size,
                      void (*destructor)(void *), void (*dealloc)(void *));

    extern  void
    __cxa_vec_delete3(void *__array_address, size_t element_size,
                      size_t padding_size, void (*destructor)(void *),
                      void (*dealloc)(void *, size_t));

    extern  void
    __cxa_vec_cctor(void *dest_array, void *src_array, size_t element_count,
                    size_t element_size, void (*constructor)(void *, void *),
                    void (*destructor)(void *));

    // 3.3.5.3 Runtime API
    // These functions are part of the C++ ABI, but they are not defined in libc++abi:
    //    int __cxa_atexit(void (*)(void *), void *, void *);
    //    void __cxa_finalize(void *);

    // 3.4 Demangler API
    extern  char *__cxa_demangle(const char *mangled_name,
                                                    char *output_buffer,
                                                    size_t *length, int *status);

    // Apple additions to support C++ 0x exception_ptr class
    // These are primitives to wrap a smart pointer around an exception object
    extern  void *__cxa_current_primary_exception() throw();
    extern  void
    __cxa_rethrow_primary_exception(void *primary_exception);
    extern  void
    __cxa_increment_exception_refcount(void *primary_exception) throw();
    extern  void
    __cxa_decrement_exception_refcount(void *primary_exception) throw();

    // Apple extension to support std::uncaught_exception()
    extern  bool __cxa_uncaught_exception() throw();
    extern  unsigned int __cxa_uncaught_exceptions() throw();

    #if defined(__linux__) || defined(__Fuchsia__)
    // Linux and Fuchsia TLS support. Not yet an official part of the Itanium ABI.
    // https://sourceware.org/glibc/wiki/Destructor%20support%20for%20thread_local%20variables
    extern  int __cxa_thread_atexit(void (*)(void *), void *,
                                                       void *) throw();
    #endif

    } // extern "C"

} // namespace __cxxabiv1

#endif // __CXXABI_H
