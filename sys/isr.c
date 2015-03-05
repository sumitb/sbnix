#include<defs.h>
#include<timer.h>
#include<idt.h>

void handler_irq0(){

    _asm_(".global irq0 \n" \
            "irq0: \n" \
            "pushq %rax;" \
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
