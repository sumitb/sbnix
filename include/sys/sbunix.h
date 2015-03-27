#ifndef __SBUNIX_H
#define __SBUNIX_H

#include <sys/defs.h>

void printk(const char *fmt, ...);
int getcsr_x();
int getcsr_y();
void gotoxy(int x,int y);
#endif
