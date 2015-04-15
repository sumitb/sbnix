#include <sys/gdt.h>
#include <sys/sched.h>
#include <sys/memory.h>
#include <sys/console.h>

uint64_t cr3_addr;

void foo() {
    printk("Hello\n");
    sys_yield();
}

void bar() {
    printk("World\n");
    sys_yield();
}

void schedule() {
    void *page_addr;
    struct task_struct task1;
    struct task_struct task2;
    /* Save current cr3 value */
    __asm__ __volatile__("movq %%cr3, %0":: "b"(cr3_addr));

    /* Task1: Hello */
    /* Init kernel stack */
    page_addr = (void *)mem_allocate();
    page_addr = KERN_MEM + page_addr;
    memset(page_addr, 0, 4096);

    current = &task1;
    task1.pml4e_base_addr = cr3_addr;
    task1.next_task = &task2;
    //task1.stack = (uint64_t*)page_addr;
    task1.kern_stack[KERNEL_STACK_SIZE - 1] = (uint64_t) &foo;
    task1.saved_kernel_rsp = &task1.kern_stack[KERNEL_STACK_SIZE - 1];

    /* Task2: World */
    page_addr = (void *)mem_allocate();
    page_addr = KERN_MEM + page_addr;
    memset(page_addr, 0, 4096);

    task2.pml4e_base_addr = cr3_addr;
    task2.next_task = &task1;
    //task2.stack = (uint64_t*)page_addr;
    task2.kern_stack[KERNEL_STACK_SIZE - 1] = (uint64_t) &bar;
    task2.saved_kernel_rsp = &task2.kern_stack[KERNEL_STACK_SIZE - 17]; /* 15 regs, 1 retq, 1 offset*/

    // Execute current task
    // __asm__ __volatile__("movq %0, %%rip"::"b"(&current.saved_kernel_stack));
    /* Invoke Task1 */
    //__asm__ __volatile__("jmpq *%0"::"b"(current->saved_kernel_stack));
    //__asm__ __volatile__("movq %0, %%rsp"::"b"(task1.saved_kernel_rsp));
    __asm__ __volatile__("movq %0, %%rsp;"::"m"(current->saved_kernel_rsp));
    __asm__ __volatile__("retq");
    return;
}
/* move two instrn ahead: 0x80 */

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
    //__asm__ __volatile__("jmpq 8(%rip)");
    /* Save rip and rsp */
    //__asm__ __volatile__("pushq 8(%rip)");
    __asm__ __volatile__("movq %%rsp, %0":"=g"(current->saved_kernel_rsp)::"memory");
    //__asm__ __volatile__("movq %%rsp, %0"::"b"(tss.rsp0));
    
    /* Do context switch */
    current = current->next_task;
    __asm__ __volatile__("movq %0, %%rsp"::"m"(current->saved_kernel_rsp));
    //__asm__ __volatile__("jmpq *%0"::"b"(current->saved_kernel_stack));

    /*tss.rsp0 Restore values */
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
    __asm__ __volatile__("retq");
    return;
}

