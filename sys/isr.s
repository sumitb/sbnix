.global handler_irq0
.global handler_irq1
.global handler_irq2
.global handler_irq3
.global handler_irq4
.global handler_irq5
.global handler_irq6
.global handler_irq7
.global handler_irq8
.global handler_irq9
.global handler_irq10
.global handler_irq11
.global handler_irq12
.global handler_irq13
.global handler_irq14
.global handler_irq15
.global handler_idt13
.global handler_idt14
.global handler_idt80
.global handler_idt126
.global handler_idt128

.macro push_registers
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %rbp
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
.endm

.macro pop_registers
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rbp
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
.endm


handler_irq0:
    push_registers
    call call_timer
    pop_registers
    iretq

handler_irq1:
    push_registers
    call kb_handler
    pop_registers
    iretq

handler_irq2:
    push_registers
    call irq2
    pop_registers
    iretq

handler_irq3:
    push_registers
    call irq3
    pop_registers
    iretq

handler_irq4:
    push_registers
    call irq4
    pop_registers
    iretq

handler_irq5:
    push_registers
    call irq5
    pop_registers
    iretq


handler_irq6:
    push_registers
    call irq6
    pop_registers
    iretq


handler_irq7:
    push_registers
    call irq7
    pop_registers
    iretq


handler_irq8:
    push_registers
    call irq8
    pop_registers
    iretq



handler_irq9:
    push_registers
    call irq9
    pop_registers
    iretq


handler_irq10:
    push_registers
    call irq10
    pop_registers
    iretq


handler_irq11:
    push_registers
    call irq11
    pop_registers
    iretq

handler_irq12:
    push_registers
    call irq12
    pop_registers
    iretq

handler_irq13:
    push_registers
    call irq13
    pop_registers
    iretq

handler_irq14:
    push_registers
    call irq14
    pop_registers
    iretq

handler_irq15:
    push_registers
    call irq15
    pop_registers
    iretq

handler_idt13:
    push_registers
    call protection_fault
    pop_registers
    iretq

handler_idt14:
    push_registers
    call page_fault
    pop_registers
    iretq

handler_idt80:
<<<<<<< HEAD
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %rbp
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
	movq %rsp, %rdi
    call syscall_handler
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rbp
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    push_registers
	movq %rsp, %rdi
    call syscall_handler
    pop_registers
	iretq

handler_idt126:
    push_registers
    call syscall_handler
    pop_registers
    iretq
	
handler_idt128:
    push_registers
    call syscall_handler
    pop_registers
    iretq





