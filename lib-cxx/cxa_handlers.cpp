//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//
// This file implements the functionality associated with the terminate_handler,
// unexpected_handler, and new_handler.
//===----------------------------------------------------------------------===//

//#include <stdexcept>
#include <inc-cxx/new>
#include <inc-cxx/exception>
//#include "abort_message.h"
#include "cxxabi.h"
#include "cxa_handlers.h"
#include "cxa_exception.h"
#include "private_typeinfo.h"
#include "inc/stdio.h"
#include "inc-cxx/stdlib.h"

namespace std
{

unexpected_handler
get_unexpected() noexcept
{
    return __cxa_unexpected_handler;
}

void
__unexpected(unexpected_handler func)
{
    func();
    // unexpected handler should not return
    cprintf("unexpected_handler unexpectedly returned");
    abort();
}

__attribute__((noreturn))
void
unexpected()
{
    __unexpected(get_unexpected());
}

terminate_handler
get_terminate() noexcept
{
    return __cxa_terminate_handler;
}

[[noreturn]] void
__terminate(terminate_handler func) noexcept
{
    try
    {
        func();
        // handler should not return
        cprintf("terminate_handler unexpectedly returned");
        abort();
    }
    catch (...)
    {
        // handler should not throw exception
        cprintf("terminate_handler unexpectedly threw an exception");
        abort();
    }
}

__attribute__((noreturn))
void
terminate() noexcept
{
    // If there might be an uncaught exception
    using namespace __cxxabiv1;
    __cxa_eh_globals* globals = __cxa_get_globals_fast();
    if (globals)
    {
        __cxa_exception* exception_header = globals->caughtExceptions;
        if (exception_header)
        {
            _Unwind_Exception* unwind_exception =
                reinterpret_cast<_Unwind_Exception*>(exception_header + 1) - 1;
            if (__isOurExceptionClass(unwind_exception))
                __terminate(exception_header->terminateHandler);
        }
    }
    __terminate(get_terminate());
}

new_handler
get_new_handler() noexcept
{
    return __cxa_new_handler;
}

}  // std
