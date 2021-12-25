//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "inc-cxx/exception"
#include "inc-cxx/new"
#include "inc-cxx/typeinfo"

#include <lib-cxx/cxxabi.h>
using namespace __cxxabiv1;
#define HAVE_DEPENDENT_EH_ABI 1

namespace std {

bool uncaught_exception() noexcept { return uncaught_exceptions() > 0; }

int uncaught_exceptions() noexcept
{
// # if _LIBCPPABI_VERSION > 1001
    return __cxa_uncaught_exceptions();
// # else
//     return __cxa_uncaught_exception() ? 1 : 0;
// # endif
}

exception_ptr::~exception_ptr() noexcept {
  __cxa_decrement_exception_refcount(__ptr_);
}

exception_ptr::exception_ptr(const exception_ptr& other) noexcept
    : __ptr_(other.__ptr_)
{
    __cxa_increment_exception_refcount(__ptr_);
}

exception_ptr& exception_ptr::operator=(const exception_ptr& other) noexcept
{
    if (__ptr_ != other.__ptr_)
    {
        __cxa_increment_exception_refcount(other.__ptr_);
        __cxa_decrement_exception_refcount(__ptr_);
        __ptr_ = other.__ptr_;
    }
    return *this;
}

nested_exception::nested_exception() noexcept
    : __ptr_(current_exception())
{
}

nested_exception::~nested_exception() noexcept
{
}

[[noreturn]]
void
nested_exception::rethrow_nested() const
{
    if (__ptr_ == nullptr)
        terminate();
    rethrow_exception(__ptr_);
}

exception_ptr current_exception() noexcept
{
    // be nicer if there was a constructor that took a ptr, then
    // this whole function would be just:
    //    return exception_ptr(__cxa_current_primary_exception());
    exception_ptr ptr;
    ptr.__ptr_ = __cxa_current_primary_exception();
    return ptr;
}

[[noreturn]]
void rethrow_exception(exception_ptr p)
{
    __cxa_rethrow_primary_exception(p.__ptr_);
    // if p.__ptr_ is NULL, above returns so we terminate
    terminate();
}


} // namespace std
