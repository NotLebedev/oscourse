/* See COPYRIGHT for copyright information. */

#include <inc/mmu.h>
#include <inc/memlayout.h>
#include <inc/trap.h>
#include <kern/macro.h>

#include <kern/picirq.h>

#ifdef CONFIG_KSPACE

.globl clock_thdlr
.type clock_thdlr, @function
clock_thdlr:
    call save_trapframe_trap
    # Set trap code for trapframe
    movl $(IRQ_OFFSET + IRQ_CLOCK), 136(%rsp)
    call trap
    jmp .

.globl timer_thdlr
.type timer_thdlr, @function
timer_thdlr:
    call save_trapframe_trap
    # Set trap code for trapframe
    movl $(IRQ_OFFSET + IRQ_TIMER), 136(%rsp)
    call trap
    jmp .

#else

# TRAPHANDLER defines a globally-visible function for handling a trap.
# It pushes a trap number onto the stack, then jumps to _alltraps.
# Use TRAPHANDLER for traps where the CPU automatically pushes an error code.
#
# You shouldn't call a TRAPHANDLER function from C, but you may
# need to _declare_ one in C (for instance, to get a function pointer
# during IDT setup).  You can declare the function with
#   void NAME();
# where NAME is the argument passed to TRAPHANDLER.

#define TRAPHANDLER(name, num) \
.globl name;                   \
.type name, @function;         \
.align 2;                      \
name:                          \
    pushq $(num);              \
    jmp _alltraps

# Use TRAPHANDLER_NOEC for traps where the CPU doesn't push an error code.
# It pushes a 0 in place of the error code, so the trap frame has the same
# format in either case.

#define TRAPHANDLER_NOEC(name, num) \
.globl name;                        \
.type name, @function;              \
.align 2;                           \
name:                               \
    pushq $0;                       \
    pushq $(num);                   \
    jmp _alltraps

.text

.globl _alltraps
.type _alltraps, @function;
.align 2
_alltraps:
  subq $16,%rsp
  movw %ds,8(%rsp)
  movw %es,(%rsp)
  PUSHA
  movl $GD_KD,%eax
  movw %ax,%ds
  movw %ax,%es
  movq %rsp, %rdi
  call trap
  jmp .

# LAB 8: Your code here
# Use TARPHANDLER or TRAPHANDLER_NOEC to setup
# all trap handlers' entry points
# (Look up their list in inc/trap.h)
TRAPHADLER(t_divide, T_DIVIDE)
TRAPHADLER(t_debug , T_DEBUG )
TRAPHADLER(t_nmi   , T_NMI   )
TRAPHADLER(t_brkpt , T_BRKPT )
TRAPHADLER(t_oflow , T_OFLOW )
TRAPHADLER(t_bound , T_BOUND )
TRAPHADLER(t_illop , T_ILLOP )
TRAPHADLER(t_device, T_DEVICE)
TRAPHADLER(t_dblflt, T_DBLFLT)
TRAPHADLER(t_tss   , T_TSS   )
TRAPHADLER(t_segnp , T_SEGNP )
TRAPHADLER(t_stack , T_STACK )
TRAPHADLER(t_gpflt , T_GPFLT )
TRAPHADLER(t_pgflt , T_PGFLT )
TRAPHADLER(t_fperr , T_FPERR )
TRAPHADLER(t_align , T_ALIGN )
TRAPHADLER(t_mchk  , T_MCHK  )
TRAPHADLER(t_simderr, T_SIMDERR)
#endif
