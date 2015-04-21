#include <sys/gdt.h>
#include <sys/sched.h>
#include <sys/memory.h>
#include <sys/console.h>

void *schedule_save;
static uint64_t avail_pid = 0;
static uint64_t num_task = 0;
struct task_struct task1, task2, task3, task4;
LIST_HEAD(runQueue);

uint64_t getCR3() {
    uint64_t cr3_addr = 0;
    
    /* Save current cr3 value */
    __asm__ __volatile__("movq %%cr3, %0":: "b"(cr3_addr));
    return cr3_addr;
}

/* Prepare kernel stack for each process
 * Save sp registers and rip
*/
void prepKernelStack(uint64_t* stack, uint64_t rip) {
    stack[KERNEL_STACK_SIZE - 1] = rip;
    /*
    stack[KERNEL_STACK_SIZE - 1] = 0x23;
    stack[KERNEL_STACK_SIZE - 2] = (uint64_t) &foo;
    stack[KERNEL_STACK_SIZE - 3] = 0x246;
    stack[KERNEL_STACK_SIZE - 4] = 0x1b;
    stack[KERNEL_STACK_SIZE - 5] = rip;
    task1.kernel_rsp = &task1.kern_stack[KERNEL_STACK_SIZE - 5];
    task2.kernel_rsp = &task2.kern_stack[KERNEL_STACK_SIZE - 22];
    */
}

struct task_struct* initTask(uint64_t entry_point) {
    /* Allocate memory for a new task_struct */
    /* TODO: kmalloc issue */
    //struct task_struct* task = (struct task_struct*)kmalloc(sizeof(struct task_struct));
    struct task_struct* task;
    if(avail_pid == 0)
        task = &task1;
    else if(avail_pid == 1)
        task = &task2;
    else if(avail_pid == 2)
        task = &task3;
    else
        task = &task4;
    task->pid = ++avail_pid;
    task->state = 0;
    task->parent = NULL;
    //task->childList = ####; /* define in sched.h and init here*/
    //task-> mm = get_mm();
    /* define get_mm in memory.h */
    task->mm = (struct mm_struct *)((char *)(&task + 1)); /* COPY: from process.c */
    
    /* Init kernel stack */
    //task.stack = (uint64_t*)page_addr;
    prepKernelStack(task->kern_stack, entry_point);
    /* 15 regs, 1 retq, 1 offset*/
    task->kernel_rsp = (uint64_t*) &task->kern_stack[KERNEL_STACK_SIZE - 1 - NUM_REGISTERS_SAVED];
    
    /* Init user stack for user processes only*/
    
    task->pml4e_base_addr = getCR3();

    return task;
}

int addTasktoQueue(struct task_struct *task) {
    if(num_task + 1 > NR_TASKS)
        return -1;
    list_add(&task->tasks, &runQueue);
    num_task++;
    return 0;
}
/*
void start_task() {
    // Execute current task
    __asm__ __volatile__("movq %0, %%rsp;"::"m"(current->kernel_rsp));
    __asm__ __volatile__("retq");
}
*/

void schedule() {
    /* Implementing a round robin scheduling policy
     * Choose the task -> next task in the list
     * If doing priority scheduling pick max prio task
     */
    //TODO: Ensure that runQueue is not empty
    /* Update global next and current task */
    nextTask = list_entry(runQueue.next, struct task_struct, tasks);
    currentTask = list_entry(runQueue.prev, struct task_struct, tasks);
   
    /* Skip schedule if next task is current task */
    if(nextTask->pid == currentTask->pid)
        return;
    
    /* Move current task from head to tail */
    list_move_tail(&nextTask->tasks, &runQueue);
    
    /*TODO: Switch cr3 */
    /* Do context switch */
    //current = current->next_task;
    
    sys_yield();
    
    /*
     __asm__ __volatile__("movq %0, %%rip"::"b"(&current.saved_kernel_stack));
    __asm__ __volatile__("jmpq *%0"::"b"(current->saved_kernel_stack));
    __asm__ __volatile__("movq %0, %%rsp"::"b"(task1.saved_kernel_rsp));
    __asm__ __volatile__("pushq 8(%rip)");
    __asm__ __volatile__("popq %rax");
    __asm__ __volatile__("movq %%rax, %0":"=g"(schedule_save)::"memory");
    __asm__ __volatile__("popq %rdx");
    __asm__ __volatile__("movq %%rdx, %0":"=g"(schedule_save)::"memory");
    schedule_save += 0x4;
    __asm__ __volatile__("jmpq 8(%rip)");
    __asm__ __volatile__("pushq 8(%rip)");
    __asm__ __volatile__("movq %%rsp, %0"::"b"(tss.rsp0));
    __asm__ __volatile__("jmpq *%0"::"b"(current->kernel_stack));
    __asm__ __volatile__("movq %0, %%rsp"::"m"(schedule_save));
    */
    /* move two instrn ahead: 0x80 */
    return;
}

void sys_yield() {
    __asm__ __volatile__("pushq %rax");
    __asm__ __volatile__("pushq %rcx");
    __asm__ __volatile__("pushq %rdx");
    __asm__ __volatile__("pushq %rbx");
    __asm__ __volatile__("pushq %rsi");
    __asm__ __volatile__("pushq %rdi");
    __asm__ __volatile__("pushq %rbp");
    __asm__ __volatile__("pushq %r8");
    __asm__ __volatile__("pushq %r9");
    __asm__ __volatile__("pushq %r10");
    __asm__ __volatile__("pushq %r11");
    __asm__ __volatile__("pushq %r12");
    __asm__ __volatile__("pushq %r13");
    __asm__ __volatile__("pushq %r14");
    __asm__ __volatile__("pushq %r15");
    /* Save rip and rsp */
    __asm__ __volatile__("movq %%rsp, %0":"=g"(currentTask->kernel_rsp)::"memory");
    
    /*tss.rsp0 Restore values */
    __asm__ __volatile__("movq %0, %%rsp"::"m"(nextTask->kernel_rsp));

    __asm__ __volatile__("popq %r15");
    __asm__ __volatile__("popq %r14");
    __asm__ __volatile__("popq %r13");
    __asm__ __volatile__("popq %r12");
    __asm__ __volatile__("popq %r11");
    __asm__ __volatile__("popq %r10");
    __asm__ __volatile__("popq %r9");
    __asm__ __volatile__("popq %r8");
    __asm__ __volatile__("popq %rbp");
    __asm__ __volatile__("popq %rdi");
    __asm__ __volatile__("popq %rsi");
    __asm__ __volatile__("popq %rbx");
    __asm__ __volatile__("popq %rdx");
    __asm__ __volatile__("popq %rcx");
    __asm__ __volatile__("popq %rax");
    //__asm__ __volatile__("add $8, %rsp"); /* retq subtracts rsp, countering for it */
	__asm__ __volatile__ ("sti");
    __asm__ __volatile__("retq");
}

