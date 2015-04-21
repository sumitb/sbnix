/*
 * Referenced from osdever.net - Bran's Kernel Dev 
 */
#include<sys/timer.h>
#include<sys/idt.h>
#include<sys/defs.h>
#include<sys/gdt.h>
#include<stdarg.h>
#include<sys/tarfs.h>
#include<sys/process.h>
#include<sys/syscall.h>
#include<sys/console.h>

extern void handler_irq0();
extern void handler_irq1();
extern void handler_idt13();
extern void handler_idt14();
extern void handler_idt80();
extern void handler_idt128();
extern void handler_idt126();

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
	
	idt_set_gate(13,(uint64_t) &handler_idt13,0x08,0x8E);
	idt_set_gate(14,(uint64_t) &handler_idt14,0x08,0x8E);
    idt_set_gate(32,(uint64_t) &handler_irq0,0x08,0x8E);
    idt_set_gate(33,(uint64_t) &handler_irq1,0x08,0x8E);
	idt_set_gate(80,(uint64_t) &handler_idt80,0x08,0xEE);
	idt_set_gate(126,(uint64_t) &handler_idt128,0x08,0xEE);
	idt_set_gate(128,(uint64_t) &handler_idt128,0x08,0xEE);
    __asm__ __volatile__ ("lidt (%0)": :"r" (&idtp));
}
volatile int ggd=1;
void syscall_handler(){
	uint64_t s_cal_no =0;
	__asm__ __volatile__("movq %%rax, %0;":"=a"(s_cal_no):);
	
	switch(s_cal_no){
		case SYS_read: //sys_read
			break;
		case SYS_write: //sys_write
			break;
		case SYS_fork:
				{
					printk("inside sys_handler\n");
					while(ggd);
					pid_t pid = sys_fork();
					__asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)pid):"cc", "memory");
				}
	}
}
