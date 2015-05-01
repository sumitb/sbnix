.include "sys/macro.s"

.global handler_isr0
.global handler_isr1
.global handler_isr2
.global handler_isr3
.global handler_isr4
.global handler_isr5
.global handler_isr6
.global handler_isr7
.global handler_isr8
.global handler_isr9
.global handler_isr10
.global handler_isr11
.global handler_isr12
.global handler_isr13
.global handler_isr14
.global handler_isr15
.global handler_isr16
.global handler_isr17
.global handler_isr18
.global handler_isr19
.global handler_isr20
.global handler_isr21
.global handler_isr22
.global handler_isr23
.global handler_isr24
.global handler_isr25
.global handler_isr26
.global handler_isr27
.global handler_isr28
.global handler_isr29
.global handler_isr30
.global handler_isr31

handler_isr13:
    push_registers
    call protection_fault
    pop_registers
    iretq

handler_isr14:
    push_registers
    call page_fault
    pop_registers
    iretq

