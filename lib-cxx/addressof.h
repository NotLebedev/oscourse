// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___MEMORY_ADDRESSOF_H
#define _LIBCPP___MEMORY_ADDRESSOF_H

namespace std {

template <class _Tp>
inline constexpr
_Tp*
addressof(_Tp& __x) noexcept
{
    return __builtin_addressof(__x);
}

template <class _Tp> _Tp* addressof(const _Tp&&) noexcept = delete;

}

#endif // _LIBCPP___MEMORY_ADDRESSOF_H
