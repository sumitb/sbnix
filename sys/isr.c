#include<sys/defs.h>
#include<sys/timer.h>
#include<sys/sbunix.h>
#include<sys/idt.h>
#if 0
void handler_irq0(){
	//printk("Interrupt\n");
/*    __asm__(".global handler_irq0 \n" \
           "handler_irq0: \n" \
*/     __asm__("pushq %rax;" \
            "pushq %rbx;" \
            "pushq %rcx;" \
            "pushq %rdx;" \
            "pushq %rsi;" \
            "pushq %rdi;" \
            "pushq %r8;" \
            "pushq %r9;" \
            "pushq %r10;" \
            "push %r11;" \
            "call call_timer;" \
            "popq %r11;" \
            "popq %r10;" \
            "popq %r9;" \
            "popq %r8;" \
            "popq %rdi;" \
            "popq %rsi;" \
            "popq %rdx;" \
            "popq %rcx;" \
            "popq %rbx;" \
            "popq %rax;" \
            "iretq;");
}
#endif
