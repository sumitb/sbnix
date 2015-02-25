#include <sys/defs.h>

static inline bool are_interrupts_enabled()
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
}

