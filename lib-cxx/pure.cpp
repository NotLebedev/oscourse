#include <inc/stdio.h>
#include <inc-cxx/stdlib.h>
#include "cxxabi.h"

extern "C" void
__cxxabiv1::__cxa_pure_virtual (void)
{
  cprintf ("pure virtual method called\n");
  abort();
}

extern "C" void
__cxxabiv1::__cxa_deleted_virtual (void)
{
  cprintf ("pure virtual method called\n");
  abort();
}
