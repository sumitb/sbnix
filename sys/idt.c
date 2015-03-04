#include <sys/defs.h>
#include <sys/idt.h>
#include <sys/timer.h>
#include <sys/gdt.h>
#include <stdarg.h>
#include<sys/tarfs.h>

extern void handler_irq0();
extern void handler_irq1();
struct idt_entry_struct
{
        uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
//        uint16_t base_mid;
  //      uint32_t base_hi;
        uint16_t sel;                 // Kernel segment selector.
        uint8_t always0;             // This must always be zero.
        uint8_t flags;               // More flags. See documentation.
        uint16_t base_mid;
        uint32_t base_hi;             // The upper 16 bits of the address to jump to.
        uint32_t reserved;
};// __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct
{
    uint16_t limit;
    uint64_t base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;



idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;


void idt_set_gate(int num, uint64_t base, uint16_t sel, uint8_t flags)
{
        idt_entries[num].base_lo = base & 0xFFFF;
        idt_entries[num].base_mid = (base >> 16) & 0xFFFF;
        idt_entries[num].base_hi = (base >> 32) & 0xFFFFFFFF;

        idt_entries[num].sel     = sel;
        idt_entries[num].always0 = 0;

        idt_entries[num].flags   = flags;
}


void lidt() {
    __asm__ __volatile__ ("lidt (%0)": :"r" (&idt_ptr));
}

void reload_idt()
{ //      init_pic();
    //    timer_set();        
        idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
        idt_ptr.base  = (uint64_t)&idt_entries;

        idt_set_gate(32, (uint64_t) &handler_irq0, 0x08, 0x8E);
        idt_set_gate(33, (uint64_t) &handler_irq1, 0x08, 0x8E);
//        __asm__volatile__("lidt (%0)": :"r" (&idt_ptr));
       lidt();


}
