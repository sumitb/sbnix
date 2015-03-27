#ifndef _IDT_H
#define _IDT_H

#include <sys/defs.h>

//bool are_interrupts_enabled();
//void lidt(void* base, uint16_t size);
<<<<<<< HEAD
void get_entry_idt(uint32_t i,uint64_t target,uint8_t flags);
=======
void get_entry_idt(int i,uint64_t target,uint8_t flags);
>>>>>>> 753b7ebf235fe9cc923bc7101c1982820983c7ac
void reload_idt();


#endif

