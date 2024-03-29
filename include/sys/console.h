#ifndef __CONSOLE_H
#define __CONSOLE_H

#include <sys/defs.h>

volatile bool scan_flag;

int printk(const char *fmt, ...);
int scank(char *buf);
void reset();
int getcsr_x();
int getcsr_y();
void gotoxy(int x, int y);
#endif
