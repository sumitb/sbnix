#ifndef __CONSOLE_H
#define __CONSOLE_H

#include <sys/defs.h>

void printk(const char *fmt, ...);
void window_scroll();
int getcsr_x();
int getcsr_y();
void gotoxy(int x, int y);
#endif
