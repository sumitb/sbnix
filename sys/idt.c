/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Interrupt Descriptor Table management
*
*  Notes: No warranty expressed or implied. Use at own risk. */
/*
 * Referenced from osdever.net - Bran's Kernel Dev
 */

#include <sys/gdt.h>
#include <sys/irq.h>
#include <sys/idt.h>
#include <sys/defs.h>
#include <sys/timer.h>

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

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
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

/* Installs the IDT */
void reload_idt() {
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit=sizeof(struct idt_entry)*256-1;
    idtp.base=(uint64_t) &idt;
	
    isrs_install();
    irq_install();
    timer_install();
    /* Points the processor's internal register to the new IDT */
    __asm__ __volatile__ ("lidt (%0)": :"r" (&idtp));
}
