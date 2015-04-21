#ifndef _IRQ_H
#define _IRQ_H

#include <sys/defs.h>

#define HZ  100

void irq_install();
void irq_install_handler(int irq, void (*handler)(void));
void irq_uninstall_handler(int irq);

#endif
