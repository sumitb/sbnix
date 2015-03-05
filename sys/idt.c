#include <sys/defs.h>
#include <stdarg.h>
#include <sys/idt.h>
#include <sys/gdt.h>
#include <sys/tarfs.h>
#include <sys/timer.h>

#define MAX_IDT 256

extern void _isr0();
extern void handler_irq0();
extern void handler_irq1();
struct idt_array
{
	uint16_t base_lo_l;
    uint16_t selector;
    uint8_t always_0;
    uint8_t flags;
	uint16_t base_lo_h;
	uint32_t base_hi;
    uint32_t reserved;
};

static struct idtr_t {
	uint16_t size;
	uint64_t addr;
}idtr;

struct idt_array idt[MAX_IDT];

void get_entry_idt(int i, uint64_t target, uint8_t flags)
{
    idt[i].base_lo_l = (target) & 0xffff;
    idt[i].flags = flags;
    idt[i].always_0 = 0;
    idt[i].selector = 8;
    idt[i].reserved = 0;
    idt[i].base_lo_h = ((target) >> 16) & 0xffff;
    idt[i].base_hi = ((target) >> 32) & 0xffffffff;
}

//struct idr_t idtr;

void reload_idt() {
    idtr.size = sizeof(struct idt_array) * MAX_IDT;
	idtr.addr = (uint64_t)&idt;
	//init_pic();
	//timer_set();
	get_entry_idt(0, (uint64_t) &_isr0, 0x8E);
	//get_entry_idt(32, (uint64_t) &handler_irq0, 0x8E);
	//get_entry_idt(33, (uint64_t) &handler_irq1, 0x8E);
	__asm __volatile__( "lidt (%0)" : : "r"(&idtr) );
	__asm __volatile__("int $0x0");
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

