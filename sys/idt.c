#include <sys/defs.h>
#include <stdarg.h>
#include <sys/idt.h>
#include <sys/timer.h>
#include <sys/gdt.h>
#include <sys/tarfs.h>
#include <sys/sbunix.h>

#define MAX_IDT 256
#define IDT_IST           (0x0000)  /*** code segment descriptor ***/
#define IDT_RESERVED      (0x0000)  /*** data segment descriptor ***/
#define IDT_TYPE          (0x0E00)  /*** conforming ***/
#define IDT_ZERO          (0x0000)  /*** conforming ***/
#define IDT_DPL0          (0x0000)  /*** descriptor privilege level 0 ***/
#define IDT_DPL1          (0x2000)  /*** descriptor privilege level 1 ***/
#define IDT_DPL2          (0x4000)  /*** descriptor privilege level 2 ***/
#define IDT_DPL3          (0x6000)  /*** descriptor privilege level 3 ***/
#define IDT_P             (0x8000)  /*** present ***/

extern void handler_irq0();
//extern void handler_irq1();
struct idt_array
{
	uint16_t offset1;
//	uint16_t offset2;
//	uint32_t offset3;
        uint16_t selector; 
        //uint8_t flags_0;        
        uint16_t flags;            
	uint16_t offset2;
	uint32_t offset3;
        uint32_t reserved;
};

static struct idtr_t {
	uint16_t size;
	uint64_t addr;
}idtr;

struct idt_array idt[MAX_IDT];

//for(int i=0; i<sizeof(idt);i++)
	
void get_entry_idt(uint32_t i,uint64_t target,uint16_t flags)
{
   idt[i].offset1 = (target) & 0xffff;
   //idt[i].flags = flags;
   idt[i].flags = IDT_IST | IDT_RESERVED | IDT_TYPE | IDT_ZERO | IDT_DPL0| IDT_P;
   idt[i].selector = 8;
  // idt[i].flags_0 = 0;
   idt[i].reserved = 0;
   idt[i].offset2 = ((target) >> 16) & 0xffff;
   idt[i].offset3 = ((target) >> 32) & 0xffffffff;
}

//struct idr_t idtr;

void reload_idt(){
	idtr.size=sizeof(idt);
	idtr.addr=(uint64_t) &idt;
	init_pic();
	timer_set();
	//get_entry_idt(32,(uint64_t) &handler_irq0,IDT_IST | IDT_RESERVED | IDT_TYPE | IDT_ZERO | IDT_DPL0| IDT_P);
	get_entry_idt(32,(uint64_t) &handler_irq0, 0x8E);
//	get_entry_idt(33,(uint64_t) &handler_irq1, 0x8E);
	__asm __volatile__( "lidt (%0)" : : "r"(&idtr) );
	printk("camehre");
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

