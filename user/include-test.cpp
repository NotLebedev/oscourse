#include <dwarf.h>
#include <types.h>
#include <partition.h>
#include <fs.h>
#include <string.h>
#include <assert.h>
#include <fd.h>
#include <x86.h>
#include <env.h>
#include <uefi.h>
#include <memlayout.h>
#include <syscall.h>
#include <lib.h>
#include <error.h>
#include <args.h>
#include <stdarg.h>
#include <kbdreg.h>
#include <elf.h>
#include <vsyscall.h>
#include <time.h>
#include <random.h>
#include <stdio.h>
#include <mmu.h>
#include <trap.h>
#include <initializer_list>
#include <cstring>
#include <typeinfo>
#include <stdlib.h>
#include <cfloat>
#include <cstdio>
#include <climits>
#include <cstdarg>
#include <type_traits>
#include <bit>
#include <source_location>
#include <cstddef>
#include <cstdlib>
#include <new>
#include <limits>
#include <version>
#include <cstdint>

int main(int argc, char **argv) {
    std::printf("Hello, world!\n");
    std::printf("Includes seem okay.\n");
    return 0;
}
