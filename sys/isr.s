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

#0x00    Division by zero
handler_isr0:
    pushq $0    # A normal ISR stub that pops a dummy error code to keep a
                # uniform stack frame
    pushq $0
    jmp isr_common_stub
    
#0x01    Debugger
handler_isr1:
    pushq $0
    pushq $1
    jmp isr_common_stub

#0x02    NMI
handler_isr2:
    pushq $0
    pushq $2
    jmp isr_common_stub

#0x03    Breakpoint
handler_isr3:
    pushq $0
    pushq $3
    jmp isr_common_stub

#0x04    Overflow
handler_isr4:
    pushq $0
    pushq $4
    jmp isr_common_stub

#0x05    Bounds
handler_isr5:
    pushq $0
    pushq $5
    jmp isr_common_stub

#0x06    Invalid Opcode
handler_isr6:
    pushq $0
    pushq $6
    jmp isr_common_stub

#0x07    Coprocessor not available
handler_isr7:
    pushq $0
    pushq $7
    jmp isr_common_stub

#0x08    Double fault
handler_isr8:
    pushq $8
    jmp isr_common_stub

#0x09    Coprocessor Segment Overrun (386 or earlier only)
handler_isr9:
    pushq $0
    pushq $9
    jmp isr_common_stub

#0x0A    Invalid Task State Segment
handler_isr10:
    pushq $10
    jmp isr_common_stub

#0x0B    Segment not present
handler_isr11:
    pushq $11
    jmp isr_common_stub

#0x0C    Stack Fault
handler_isr12:
    pushq $12
    jmp isr_common_stub

#0x0D    General protection fault
handler_isr13:
    pushq $13
    jmp isr_common_stub

#0x0E    Page fault
handler_isr14:
    pushq $14
    jmp isr_common_stub

#0x0F    reserved
handler_isr15:
    pushq $0
    pushq $15
    jmp isr_common_stub

#0x10    Math Fault
handler_isr16:
    pushq $0
    pushq $16
    jmp isr_common_stub

#0x11    Alignment Check
handler_isr17:
    pushq $0
    pushq $17
    jmp isr_common_stub

#0x12    Machine Check
handler_isr18:
    pushq $0
    pushq $18
    jmp isr_common_stub

#0x13    SIMD Floating-Point Exception
handler_isr19:
    pushq $0
    pushq $19
    jmp isr_common_stub

handler_isr20:
    pushq $0
    pushq $20
    jmp isr_common_stub

handler_isr21:
    pushq $0
    pushq $21
    jmp isr_common_stub

handler_isr22:
    pushq $0
    pushq $22
    jmp isr_common_stub

handler_isr23:
    pushq $0
    pushq $23
    jmp isr_common_stub

handler_isr24:
    pushq $0
    pushq $24
    jmp isr_common_stub

handler_isr25:
    pushq $0
    pushq $25
    jmp isr_common_stub

handler_isr26:
    pushq $0
    pushq $26
    jmp isr_common_stub

handler_isr27:
    pushq $0
    pushq $27
    jmp isr_common_stub

handler_isr28:
    pushq $0
    pushq $28
    jmp isr_common_stub

handler_isr29:
    pushq $0
    pushq $29
    jmp isr_common_stub

handler_isr30:
    pushq $0
    pushq $30
    jmp isr_common_stub

handler_isr31:
    pushq $0
    pushq $31
    jmp isr_common_stub

isr_common_stub:
    # Pass to fault_handler:
    # pointer to interrupt number
    pushq %rdi
    pushq %rsi

    #Index of the Interrupt_No in Stack
    movq %rsp, %rdi
    addq $0x10, %rdi
    movq %rsp, %rsi
    addq $0x18, %rsi
    push_registers
    call fault_handler
    pop_registers
    popq %rsi
    popq %rdi
    addq $0x10, %rsp   # Cleans up the pushed error code and pushed ISR number
    iretq

