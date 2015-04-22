#include <sys/sched.h>
#include <sys/memory.h>
#include <sys/console.h>

static uint64_t num_task = 0;
LIST_HEAD(runQueue);

/*
void start_task() {
    // Execute current task
    __asm__ __volatile__("movq %0, %%rsp;"::"m"(current->kernel_rsp));
    __asm__ __volatile__("retq");
}
*/

struct task_struct *getCurrentTask() {
    return list_entry(runQueue.prev, struct task_struct, tasks);
}

int addTasktoQueue(struct task_struct *task) {
    if(num_task + 1 > NR_TASKS)
        return -1;
    list_add(&task->tasks, &runQueue);
    num_task++;
    return 0;
}

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
	__asm __volatile("movq %0, %%cr3":: "a"(currentTask->cr3_address));
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
    __asm__ __volatile__("add $8, %rsp"); /* retq subtracts rsp, countering for it */
    __asm__ __volatile__("add $8, %rsp"); /* retq subtracts rsp, countering for it */
	__asm__ __volatile__ ("sti");
    __asm__ __volatile__("retq");
}

