#ifndef __SCHED_H
#define __SCHED_H

#include <sys/defs.h>
#include <sys/list.h>

#define NR_TASKS    128
#define NUM_REGISTERS_SAVED 15
#define KERNEL_STACK_SIZE   64
#define STACK_MAGIC 0xdeadbeef

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

struct vm_struct {
    uint64_t vm_start;
    uint64_t vm_end;
    uint64_t vm_mmsz;
    uint64_t vm_file;
    struct vm_struct *vm_next;
    struct mm_struct *vm_mm;
};

struct mm_struct {
    struct vm_struct *vma_addr;
	uint16_t cnt;
};

struct task_struct {
    volatile int64_t state;    /* -1 unrunnable, 0 runnable, >0 stopped */
    uint32_t flags;     /* per process flags, defined below */

    uint64_t *stack;     /* user stack */
    uint64_t kern_stack[KERNEL_STACK_SIZE];     /* maintain one kernel stack per task */
    uint64_t pid;

    uint64_t *kernel_rsp;
    uint64_t pml4e_base_addr;
    struct mm_struct *mm;
    struct task_struct *parent;
    struct list_head tasks;
    struct list_head children;      /* list of my children */
};

//struct kernel_stack kern_stack[NR_TASKS];
//struct kernel_stack *stack;     /* maintain one kernel stack per task */
//struct tss_struct tss_r;
//struct task_struct *task[NR_TASKS];
struct task_struct *currentTask;
struct task_struct *nextTask;

struct task_struct *initTask(uint64_t entry_point);
int addTasktoQueue(struct task_struct *task);
void sys_yield();
void schedule();
#endif

