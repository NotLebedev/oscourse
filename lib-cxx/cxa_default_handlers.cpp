//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//
// This file implements the default terminate_handler, unexpected_handler and
// new_handler.
//===----------------------------------------------------------------------===//

#include <inc-cxx/exception>
#include <inc-cxx/stdlib.h>
#include <inc-cxx/new>
#include <inc/stdio.h>
#include "cxxabi.h"
#include "cxa_handlers.h"
#include "cxa_exception.h"
#include "private_typeinfo.h"

static const char* cause = "uncaught";

__attribute__((noreturn))
static void demangling_terminate_handler()
{
    using namespace __cxxabiv1;
    __cxa_eh_globals* globals = __cxa_get_globals_fast();
    if (globals)
    {
        __cxa_exception* exception_header = globals->caughtExceptions;
        // If there is an uncaught exception
        if (exception_header)
        {
            _Unwind_Exception* unwind_exception =
                reinterpret_cast<_Unwind_Exception*>(exception_header + 1) - 1;
            if (__isOurExceptionClass(unwind_exception))
            {
                void* thrown_object =
                    __getExceptionClass(unwind_exception) == kOurDependentExceptionClass ?
                        ((__cxa_dependent_exception*)exception_header)->primaryException :
                        exception_header + 1;
                const __shim_type_info* thrown_type =
                    static_cast<const __shim_type_info*>(exception_header->exceptionType);

                const char* name = thrown_type->name();

                const __shim_type_info* catch_type =
                    static_cast<const __shim_type_info*>(&typeid(std::exception));

                if (catch_type->can_catch(thrown_type, thrown_object)) {
                    const std::exception* e = static_cast<const std::exception*>(thrown_object);
                    printf("terminating with %s exception of type %s: %s\n",
                                  cause, name, e->what());
                   abort();
                } else {
                    printf("terminating with %s exception of type %s\n",
                                   cause, name);
                    abort();
                }
            } else {
                // Else we're terminating with a foreign exception
                printf("terminating with %s foreign exception\n", cause);
                abort();
            }
        }
    }
    // Else just note that we're terminating
    printf("terminating\n");
    abort();
}

__attribute__((noreturn))
static void demangling_unexpected_handler()
{
    cause = "unexpected";
    std::terminate();
}

static constexpr std::terminate_handler default_terminate_handler = demangling_terminate_handler;
static constexpr std::terminate_handler default_unexpected_handler = demangling_unexpected_handler;
// static constexpr std::terminate_handler default_terminate_handler = ::abort;
// static constexpr std::terminate_handler default_unexpected_handler = std::terminate;

//
// Global variables that hold the pointers to the current handler
//
std::terminate_handler __cxa_terminate_handler = default_terminate_handler;

std::unexpected_handler __cxa_unexpected_handler = default_unexpected_handler;

std::new_handler __cxa_new_handler = 0;

namespace std
{

unexpected_handler
set_unexpected(unexpected_handler func) noexcept
{
    if (func == 0)
        func = default_unexpected_handler;
    unexpected_handler old = __cxa_unexpected_handler;
    __cxa_unexpected_handler = func;
    return old;
}

terminate_handler
set_terminate(terminate_handler func) noexcept
{
    if (func == 0)
        func = default_terminate_handler;
    terminate_handler old = __cxa_terminate_handler;
    __cxa_terminate_handler = func;
    return old;
}

new_handler
set_new_handler(new_handler handler) noexcept
{
    new_handler old = __cxa_new_handler;
    __cxa_new_handler = handler;
    return old;
}

}
