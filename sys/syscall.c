#include <string.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <sys/sched.h>
#include <sys/tarfs.h>
#include <sys/syscall.h>
#include <sys/console.h>

volatile int ggd=0;

int64_t sys_write(uint64_t fildes, char *buf, uint64_t size) {
    char tem_buf[size];

    strncpy(tem_buf,buf,size);
    tem_buf[size] = '\0';
    return printk(tem_buf);
    /*if(fildes==1 || fildes==2) {
    }
    return -1;
    */
}

void syscall_handler(){
	uint64_t s_cal_no, param_1, param_2, param_3;
	__asm__ __volatile__("movq %%rax, %0;":"=a"(s_cal_no):);
	__asm__ __volatile__("movq %%rbx, %0;":"=a"(param_1):);
	__asm__ __volatile__("movq %%rsi, %0;":"=a"(param_2):);
	__asm__ __volatile__("movq %%rdx, %0;":"=a"(param_3):);
	
	switch(s_cal_no){
		case SYS_read: //sys_read
			break;
		case SYS_write: //sys_write
            sys_write((uint64_t)param_1, (char*)param_2, (uint64_t)param_3);
            break;
		case SYS_fork:
            {
                while(ggd);
                pid_t pid = sys_fork();
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)pid):"cc", "memory");
            }
            break;
		case SYS_exit:
            {
                uint64_t status;
                __asm__ __volatile__("movq %%rbx, %0;" :"=b"(status):);
                //TODO call exit
            }
			break;
		case SYS_open:
            {
                uint16_t fd = 0;
                while(ggd);
                //TODO call exit
                //fd = sys_open((const char *)param_1, (int)param_2);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)fd):"cc", "memory");
            }
            break;
	}
}

