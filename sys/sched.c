#include <sys/gdt.h>
#include <sys/sched.h>
#include <sys/memory.h>
#include <sys/console.h>

static uint64_t num_task = 0;
LIST_HEAD(runQueue);
LIST_HEAD(waitQueue);

/*
void start_task() {
    // Execute current task
    __asm__ __volatile__("movq %0, %%rsp;"::"m"(current->kernel_rsp));
    __asm__ __volatile__("retq");
}
*/
struct task_struct *getNextTask() {
    return list_entry(runQueue.next, struct task_struct, tasks);
}

struct task_struct *getCurrentTask() {
    return list_entry(runQueue.prev, struct task_struct, tasks);
}

int addTasktoQueueHead(struct task_struct *task) {
    if(num_task + 1 > NR_TASKS)
        return -1;
    list_add(&task->tasks, &runQueue);
    num_task++;
    return 0;
}

int addTasktoQueue(struct task_struct *task) {
    if(num_task + 1 > NR_TASKS)
        return -1;
    list_add_tail(&task->tasks, &runQueue);
    num_task++;
    return 0;
}

void printSchedulerQueue() {
     struct task_struct *process = NULL;
     printk("PCB READY QUEUE: \n");
     list_for_each_entry(process, &runQueue, tasks) {
         printk("PID: %d\n", process->pid);
     }
}

int64_t sys_getpid(void) {
    return currentTask->pid;
}

int64_t sys_getppid(void) {
    return currentTask->ppid;
}

int64_t sys_exit(int error_code) {
    struct task_struct *process = getCurrentTask();
    
    /* Delete current task from running queue */
    list_del(&process->tasks);
    num_task--;
    schedule();
    return -1;
}

int64_t sys_sleep(uint64_t seconds) {
    return -1;
}

int64_t sys_waitpid(pid_t pid, int *status_addr, int options) {
    struct task_struct *process = NULL;
    /* If child pid exists, wait */
    list_for_each_entry(process, &runQueue, tasks) {
        if(process->pid == pid) {
            /* Move current task from head to tail */
            list_move(&nextTask->tasks, &waitQueue);
            schedule();
            return pid;
        }
    }
    return -1;
}

int64_t sys_kill(int pid, int sig) {
    struct task_struct *process = NULL;
    /* If child pid exists, kill */
    list_for_each_entry(process, &runQueue, tasks) {
        if(process->pid == pid) {
            /* Delete child task from running queue */
            list_del(&process->tasks);
            num_task--;
            return 0;
        }
    }
    list_for_each_entry(process, &waitQueue, tasks) {
        if(process->pid == pid) {
            /* Delete child task from wait queue */
            list_del(&process->tasks);
            num_task--;
            return 0;
        }
    }
    return -1;
}

/*
 * NOTE: Never put pritnk calls in schdeuler
 */
void schedule() {
    /* Implementing a round robin scheduling policy
     * Choose the task -> next task in the list
     * If doing priority scheduling pick max prio task
     */
    /* Skip schedule if:
     *      1. running queue is empty
     *      2. only a single process is in queue
     */
    //if(list_empty(&runQueue) || list_is_singular(&runQueue))
    if(list_empty(&runQueue))
        return;

    /* Update global next and current task */
    nextTask = getNextTask();
    currentTask = getCurrentTask();
    
    /* Move current task from head to tail */
    list_move_tail(&nextTask->tasks, &runQueue);
    
    /* Do context switch */
    sys_yield();
    
    return;
}

void sys_yield() {
    /*
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
    */
    /* Save rsp */
    //__asm__ __volatile__("movq %%rsp, %0":"=g"(currentTask->kernel_rsp)::"memory");
    //__asm__ __volatile__("movq %0, %%rsp"::"m"(nextTask->kernel_rsp));
    
    /* Switch cr3 */
	__asm __volatile("movq %0, %%cr3":: "a"(nextTask->cr3_address));
    /* Restore tss.rsp0 values */
    __asm__ __volatile__("movq %0, %%rsp"::"m"(nextTask->kernel_rsp));
	tss.rsp0 = (uint64_t)&(nextTask->kstack[KERNEL_STACK_SIZE - 1]);

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
    __asm__ __volatile__("popq %rdx");
    __asm__ __volatile__("popq %rcx");
    __asm__ __volatile__("popq %rbx");
    __asm__ __volatile__("popq %rax");
    //__asm__ __volatile__("add $8, %rsp"); /* retq subtracts rsp, countering for it */
	//__asm__ __volatile__ ("sti");
    __asm__ __volatile__("iretq");
}

