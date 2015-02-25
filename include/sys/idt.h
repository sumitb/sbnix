#ifndef _IDT_H
#define _IDT_H

#include <sys/defs.h>

bool are_interrupts_enabled();
void lidt(void* base, uint16_t size);

#endif

