#include <sys/defs.h>
#include <stdarg.h>
#include <sys/idt.h>
#include <sys/timer.h>
#include <sys/gdt.h>
#include <sys/tarfs.h>

#define MAX_IDT 256

extern void handler_irq0();
extern void handler_irq1();
struct idt_array
{
	uint16_t offset1;
	uint16_t offset2;
	uint16_t offset3;
        uint16_t selector; 
        uint8_t flags_0;        
        uint8_t flags;            
        uint32_t reserved;
};

struct idtr_t {
	uint16_t size;
	uint64_t addr;
}idtr;

struct idt_array idt[MAX_IDT];

void get_entry_idt(int i,uint64_t target,uint8_t flags)
{
   idt[i].offset1 = (target) & 0x0000ffff;
   idt[i].flags = flags;
   idt[i].flags_0 = 0;
   idt[i].selector = 8;
   idt[i].reserved = 0;
   idt[i].offset2 = ((target) >> 16) & 0x0000ffff;
   idt[i].offset3 = ((target) >> 32) & 0xffffffff;
}

//struct idr_t idtr;

void reload_idt(){
	idtr.size=sizeof(idt);
	idtr.addr=(uint64_t)idt;
	init_pic();
	timer_set();
	get_entry_idt(32,(uint64_t) &handler_irq0, 0x8E);
	get_entry_idt(33,(uint64_t) &handler_irq1, 0x8E);
	__asm __volatile__( "lidt (%0)" : : "r"(&idtr) );
}

/*static inline bool are_interrupts_enabled()
{
    unsigned long flags;
    __asm volatile ( "pushf\n\t"
                   "pop %0"
                  : "=g"(flags) );
    return flags & (1 << 9);
}

static inline void lidt(void* base, uint16_t size)
{
    struct
    {
        uint16_t length;
        uint32_t base;
    } __attribute__((packed)) IDTR;

    IDTR.length = size;
    IDTR.base = (uint64_t) base;
    __asm ( "lidt (%0)" : : "p"(&IDTR) );
}*/

