#ifndef __CONSOLE_H
#define __CONSOLE_H

#include <sys/defs.h>

int printk(const char *fmt, ...);
//void printk(const char *fmt, ...);
//void window_scroll();
int getcsr_x();
int getcsr_y();
void clr_scr();
int scank(char *buf);
void gotoxy(int x, int y);
#endif
