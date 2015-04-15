#ifndef __SCHED_H
#define __SCHED_H

#include <sys/defs.h>
#define NR_TASKS    128
#define KERNEL_STACK_SIZE   64

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

struct task_struct {
    volatile long state;    /* -1 unrunnable, 0 runnable, >0 stopped */
    unsigned int flags;     /* per process flags, defined below */

    //uint64_t *stack;     /* maintain one kernel stack per task */
    uint64_t kern_stack[KERNEL_STACK_SIZE];     /* maintain one kernel stack per task */
    void *saved_kernel_rsp;
    unsigned long pml4e_base_addr;

    struct task_struct *next_task;
};

//struct kernel_stack kern_stack[NR_TASKS];
//struct kernel_stack *stack;     /* maintain one kernel stack per task */
//struct tss_struct tss_r;
struct task_struct *task[NR_TASKS];
struct task_struct *current;

void sys_yield();
void schedule();
#endif

