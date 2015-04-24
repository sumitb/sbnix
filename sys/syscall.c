#include <stdlib.h>
#include <sys/defs.h>
#include <sys/sched.h>
#include <sys/syscall.h>
#include <sys/console.h>

volatile int ggd=1;

int64_t sys_write(int fildes, char *buf, int size) {
    ((char*)(buf))[size] = '\0';
    return printk(buf);
    /*if(fildes==1 || fildes==2) {
    }
    return -1;
    */
}

void syscall_handler(){
	uint64_t s_cal_no, param_1, param_2, param_3;
	__asm__ __volatile__("movq %%rax, %0;":"=a"(s_cal_no):);
	__asm__ __volatile__("movq %%rdi, %0;":"=a"(param_1):);
	__asm__ __volatile__("movq %%rsi, %0;":"=a"(param_2):);
	__asm__ __volatile__("movq %%rdx, %0;":"=a"(param_3):);
	
	switch(s_cal_no){
		case SYS_read: //sys_read
			break;
		case SYS_write: //sys_write
            sys_write((int)param_1, (char*)param_2, (int)param_3);
            break;
		case SYS_fork:
				{
					//printk("inside sys_handler\n");
					while(ggd);
					pid_t pid = sys_fork();
					__asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)pid):"cc", "memory");
				}
	}
}

