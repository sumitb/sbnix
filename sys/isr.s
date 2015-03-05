

.macro PUSHA
        pushq %rdi
        pushq %rax
        pushq %rbx
        pushq %rcx
        pushq %rdx
        pushq %rbp
        pushq %rsi
        pushq %r8
        pushq %r9
        pushq %r10
        pushq %r11
        pushq %r12
        pushq %r13
        pushq %r14
        pushq %r15
.endm

.macro POPA
        popq %r15
        popq %r14
        popq %r13
        popq %r12
        popq %r11
        popq %r10
        popq %r9
        popq %r8
        popq %rsi
        popq %rbp
        popq %rdx
        popq %rcx
        popq %rbx
        popq %rax
        popq %rdi
.endm
.text
.global handler_irq0
handler_irq0:
        PUSHA
        movq %rsp, %rdi
        callq call_timer
        POPA
        iretq

