/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Interrupt Service Routines installer and exceptions
*
*  Notes: No warranty expressed or implied. Use at own risk. */

#include <sys/idt.h>
#include <sys/irq.h>
#include <sys/memory.h>
#include <sys/console.h>

/* These are function prototypes for all of the exception
*  handlers: The first 32 entries in the IDT are reserved
*  by Intel, and are designed to service exceptions! */
extern void handler_isr0();
extern void handler_isr1();
extern void handler_isr2();
extern void handler_isr3();
extern void handler_isr4();
extern void handler_isr5();
extern void handler_isr6();
extern void handler_isr7();
extern void handler_isr8();
extern void handler_isr9();
extern void handler_isr10();
extern void handler_isr11();
extern void handler_isr12();
extern void handler_isr13();
extern void handler_isr14();
extern void handler_isr15();
extern void handler_isr16();
extern void handler_isr17();
extern void handler_isr18();
extern void handler_isr19();
extern void handler_isr20();
extern void handler_isr21();
extern void handler_isr22();
extern void handler_isr23();
extern void handler_isr24();
extern void handler_isr25();
extern void handler_isr26();
extern void handler_isr27();
extern void handler_isr28();
extern void handler_isr29();
extern void handler_isr30();
extern void handler_isr31();

/* This is a very repetitive function... it's not hard, it's
*  just annoying. As you can see, we set the first 32 entries
*  in the IDT to the first 32 ISRs. We can't use a for loop
*  for this, because there is no way to get the function names
*  that correspond to that given entry. We set the access
*  flags to 0x8E. This means that the entry is present, is
*  running in ring 0 (kernel level), and has the lower 5 bits
*  set to the required '14', which is represented by 'E' in
*  hex. */
void isrs_install()
{
    idt_set_gate(0, (uint64_t) &handler_isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint64_t) &handler_isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint64_t) &handler_isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint64_t) &handler_isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint64_t) &handler_isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint64_t) &handler_isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint64_t) &handler_isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint64_t) &handler_isr7, 0x08, 0x8E);

    idt_set_gate(8, (uint64_t) &handler_isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint64_t) &handler_isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint64_t) &handler_isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint64_t) &handler_isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint64_t) &handler_isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint64_t) &handler_isr13, 0x08, 0x8E);
	idt_set_gate(14, (uint64_t) &handler_isr14, 0x08, 0x8E);
    idt_set_gate(13, (uint64_t) &handler_isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint64_t) &handler_isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint64_t) &handler_isr15, 0x08, 0x8E);

    idt_set_gate(16, (uint64_t) &handler_isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint64_t) &handler_isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint64_t) &handler_isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint64_t) &handler_isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint64_t) &handler_isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint64_t) &handler_isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint64_t) &handler_isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint64_t) &handler_isr23, 0x08, 0x8E);

    idt_set_gate(24, (uint64_t) &handler_isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint64_t) &handler_isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint64_t) &handler_isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint64_t) &handler_isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint64_t) &handler_isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint64_t) &handler_isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint64_t) &handler_isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint64_t) &handler_isr31, 0x08, 0x8E);
}

/* This is a simple string array. It contains the message that
*  corresponds to each and every exception. We get the correct
*  message by accessing like:
*  exception_message[interrupt_number] */
const char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/* All of our Exception handling Interrupt Service Routines will
*  point to this function. This will tell us what exception has
*  happened! Right now, we simply halt the system by hitting an
*  endless loop. All ISRs disable interrupts while they are being
*  serviced as a 'locking' mechanism to prevent an IRQ from
*  happening and messing up kernel data structures */
void fault_handler(uint64_t *r, uint64_t *err_code)
{
    if (*r == 14)
    {
        page_fault(*err_code);
        return;
        
    }
    if (*r < 32)
    {
        printk("%s\n", exception_messages[*r]);
        printk("Exception. System Halted!\n");
        for (;;);
    }
}
