#include<sys/timer.h>
#include<sys/idt.h>
#include<sys/defs.h>
#include<sys/gdt.h>
#include<stdarg.h>
#include<sys/tarfs.h>
#include<sys/process.h>
#include<sys/syscall.h>
#include<sys/console.h>

volatile int ggd=1;
void syscall_handler(){
	uint64_t s_cal_no =0;
	__asm__ __volatile__("movq %%rax, %0;":"=a"(s_cal_no):);
	
	switch(s_cal_no){
		case SYS_read: //sys_read
			break;
		case SYS_write: //sys_write
			break;
		case SYS_fork:
				{
					printk("inside sys_handler\n");
					while(ggd);
					pid_t pid = sys_fork();
					__asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)pid):"cc", "memory");
				}
	}
}

