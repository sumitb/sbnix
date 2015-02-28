#ifndef TIMER_H
#define TIMER_H


#include <sys/defs.h>
//void outb(uint16_t port, uint8_t val);
//void outportb(int port,uint8_t data);
//uint8_t inb(uint16_t port);
void timer_set();
void init_pic();
void call_timer();

#endif
