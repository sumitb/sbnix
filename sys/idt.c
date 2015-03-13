/*
 * Referenced from osdever.net - Bran's Kernel Dev 
 */
#include<sys/timer.h>
#include<sys/idt.h>
#include<sys/defs.h>
#include<sys/gdt.h>
#include<stdarg.h>
#include<sys/tarfs.h>

extern void handler_irq0();
extern void handler_irq1();

/* Defines an IDT entry */
struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;        /* Our kernel segment goes here! */
    unsigned char always0;     /* This will ALWAYS be set to 0! */
    unsigned char flags;       /* Set using the above table! */
    unsigned short base_mid;
    unsigned int base_high;
    unsigned int reserved;
} __attribute__((packed));


struct idt_ptr
{
    unsigned short limit;
    unsigned long base;
} __attribute__((packed));


struct idt_entry idt[256];
struct idt_ptr idtp;

void idt_set_gate(int num, unsigned long base, unsigned short sel, unsigned char flags) {
    idt[num].base_lo=base & 0xffff;
    idt[num].base_mid=(base>>16) & 0xffff;
    idt[num].base_high=(base>>32) & 0xffffffff;
    idt[num].sel=sel;
    idt[num].always0=0;
    idt[num].flags=flags;
}

void reload_idt() {
    idtp.limit=sizeof(struct idt_entry)*256-1;
    idtp.base=(uint64_t) &idt;

    idt_set_gate(32,(uint64_t) &handler_irq0,0x08,0x8E);
    idt_set_gate(33,(uint64_t) &handler_irq1,0x08,0x8E);
    __asm__ __volatile__ ("lidt (%0)": :"r" (&idtp));
}

