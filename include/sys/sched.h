#ifndef __SCHED_H
#define __SCHED_H

#include <stdlib.h>
#include <sys/defs.h>
#include <sys/list.h>

#define SIGKILL             9
#define NR_TASKS            128
#define TRAP_SIZE           5
#define TSS_OFFSET          1
#define DIR_LEN             48
#define BINARY_LEN          24
#define USER_STACK_SIZE     512
#define KERNEL_STACK_SIZE   512
#define NUM_REGISTERS_SAVED 15
#define STACK_MAGIC         0xdeadbeef
#define STACK_OFFSET        KERNEL_STACK_SIZE - NUM_REGISTERS_SAVED - TRAP_SIZE - TSS_OFFSET

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
	uint64_t vm_offset;
    struct vm_struct *vm_next;
    struct mm_struct *vm_mm;
};
typedef struct vm_struct vma;

struct mm_struct {
    struct vm_struct *vma_addr;
	uint16_t cnt;
};

struct heap_mem {
    uint64_t bump_ptr;
    uint64_t rem_size;
    uint64_t end_addr;
};

struct task_struct {
    volatile int16_t state;    /* -1 unrunnable, 0 runnable, >0 stopped */

    uint16_t pid;
    uint16_t ppid;
    char dir_path[DIR_LEN];
    char bin_name[BINARY_LEN];
    bool is_sleep;
    int64_t sleep_time;
    
    uint64_t *stack;     /* user stack */
    uint64_t kstack[KERNEL_STACK_SIZE];     /* maintain one kernel stack per task */

    uint64_t *kernel_rsp;
    uint64_t cr3_address;
    uint64_t pml4e_addr;            /* REML: Virtual address of cr3 */
    uint64_t entry_pt;
    struct File fd[25];
	uint64_t dup_arr[25];
	int fd_cnt;
	
    vma *heap_vma;
    struct heap_mem heap;
    struct mm_struct *mm;
    struct list_head tasks;
};

//struct kernel_stack kern_stack[NR_TASKS];
//struct kernel_stack *stack;     /* maintain one kernel stack per task */
//struct tss_struct tss_r;
//struct task_struct *task[NR_TASKS];
struct task_struct *currentTask;
struct task_struct *nextTask;

/* Pre-increment available pid while assigning */
uint16_t avail_pid;

/* Scheduler funnctions */
void awake();
void schedule();
void sys_yield();
void awakepid(pid_t pid);
void printSchedulerQueue();
struct task_struct *getNextTask();
struct task_struct *getCurrentTask();
int addTasktoQueue(struct task_struct *task);
int addTasktoQueueHead(struct task_struct *task);

/* Process functions */
struct task_struct *initTask(uint64_t entry_point);
struct task_struct *create_process(const char *binary);
void init_process(uint64_t *stack);
void allocate(uint64_t pml4e_addr, void * addr, int len);
vma* allocate_vma(vma **vma_head);
#endif

