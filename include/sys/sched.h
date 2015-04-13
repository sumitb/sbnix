#ifndef __SCHED_H
#define __SCHED_H

#include <sys/defs.h>
#define NR_TASKS 128

/* A struct for saving and restoring processor state info. */
/* Don't confuse with tss segment, defined in sys/gdt.h */
/* Note: hardware task swtching won't work in long mode */
struct tss_struct {
    uint64_t rip;
    uint64_t rflags;
    uint64_t rax,rcx,rdx,rbx;
    uint64_t rsi,rdi,rbp;
    uint64_t r8,r9,r10,r11,r12,r13,r14,r15;
};

struct user_stack {
    char stack[4096];
};

struct kernel_stack {
    char stack[4096];
};

struct task_struct {
    volatile long state;    /* -1 unrunnable, 0 runnable, >0 stopped */
    unsigned int flags;     /* per process flags, defined below */

    void *saved_kernel_stack;
    struct kernel_stack *stack;     /* maintain one kernel stack per task */
    unsigned long pml4e_base_addr;

    struct task_struct *next_task, *prev_task;
    struct tss_struct tss_r;
};

struct kernel_stack kern_stack[NR_TASKS];
struct task_struct *task[NR_TASKS];
struct task_struct *current;

void sys_yield();
void schedule();
#endif
