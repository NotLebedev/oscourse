//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//
// This file implements the "Auxiliary Runtime APIs"
// https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html#cxx-aux
//===----------------------------------------------------------------------===//

#include <inc-cxx/new>
#include <inc-cxx/typeinfo>
#include <inc-cxx/stdlib.h>

namespace __cxxabiv1 {
extern "C" {
[[noreturn]] void __cxa_bad_cast(void) {
  abort();
}

[[noreturn]] void __cxa_bad_typeid(void) {
  abort();
}

[[noreturn]] void
__cxa_throw_bad_array_new_length(void) {
  abort();
}
} // extern "C"
} // abi
