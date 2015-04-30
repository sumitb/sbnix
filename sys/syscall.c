#include <stdlib.h>
#include <string.h>
#include <sys/defs.h>
#include <sys/sched.h>
#include <sys/tarfs.h>
#include <sys/syscall.h>
#include <sys/console.h>
#include <sys/memory.h>

volatile int ggd=1;


int64_t sys_write(uint64_t fildes, const char *buf, uint64_t size) {

    char tem_buf[size];
    strncpy(tem_buf,buf,size);
    tem_buf[size] = '\0';
    return printk(tem_buf);

    /*if(fildes==1 || fildes==2) {
    }
    return -1;
    */
}

int64_t sys_read(uint64_t fildes, char *buf, uint64_t size) {

    if(size>0){
    	char tem_buf[size];
    	tem_buf[size] = '\0';
    	uint64_t len=0;
    	len = scank(tem_buf);
		if(len>size)
    		len=size;	
    	strncpy(buf,tem_buf,len);
    	buf[len]='\0';
    	return len;
    }
    return 0;
}

pid_t sys_getpid(){

    return (running_proc.process.process_id);
}

pid_t sys_getppid(){

    return(running_proc.process.parent_id);

}

void syscall_handler(){
	uint64_t s_cal_no, param_1, param_2, param_3;
	__asm__ __volatile__("movq %%rax, %0;":"=a"(s_cal_no):);
//	__asm__ __volatile__("movq %%rdi, %0;":"=a"(param_1):);
	__asm__ __volatile__("movq %%rsi, %0;":"=a"(param_2):);
	__asm__ __volatile__("movq %%rbx, %0;":"=a"(param_1):);
//	__asm__ __volatile__("movq %%rcx, %0;":"=a"(param_2):);
	__asm__ __volatile__("movq %%rdx, %0;":"=a"(param_3):);
	
	switch(s_cal_no){
		case SYS_read: //sys_read
		{
			int num=0;
		//	while(ggd);	
			__asm__ __volatile__ ("sti");
            		num = sys_read((uint64_t)param_1, (char*)param_2, (uint64_t)param_3);
			__asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)num):"cc", "memory");
            		break;
		}
		case SYS_write: //sys_write
		{
			//while(ggd);	
            		sys_write((uint64_t)param_1, (char*)param_2, (uint64_t)param_3);
            		break;
		}
		case SYS_fork:
		{
			//printk("inside sys_handler\n");
			//while(ggd);
			pid_t pid = sys_fork();
			__asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)pid):"cc", "memory");
			break;
		}
		case SYS_exit:
		{
			uint64_t status;
        		__asm__ __volatile__("movq %%rbx, %0;" :"=b"(status):);
			//TODO call exit	
			break;
		}
		case SYS_open:
		{
			uint16_t fd;
//			while(ggd);
			fd = sys_open((const char *)param_1, (int)param_2);
			__asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)fd):"cc", "memory");
            break;
		}
		case SYS_brk:
		{
			uint64_t addr;
			while(ggd);
			addr = sys_brk(param_1);
			__asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)addr):"cc", "memory");
			break;
		}
		case SYS_getcwd:
		{
			char *dir_name;
			dir_name = sys_getcwd();
			__asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)dir_name):"cc", "memory");
			break;
		}
		case SYS_getpid:
		{
			pid_t pid = sys_getpid();
			__asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)pid):"cc", "memory");
			break;
		}
		case SYS_getppid:
		{
			pid_t pid = sys_getppid();
			__asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)pid):"cc", "memory");
			break;
		}
		case SYS_execve:
		{
			
			break;
		}
	}
}


void irq2(){
printk("handler for hardware interrupt 2 \n");
}

void irq3(){
printk("handler for hardware interrupt \n");
}

void irq4(){
printk("handler for hardware interrupt  \n");
}

void irq5(){
printk("handler for hardware interrupt \n");
}

void irq6(){
printk("handler for hardware interrupt  \n");
}

void irq7(){
printk("handler for hardware interrupt \n");
}

void irq8(){
printk("handler for hardware interrupt \n");
}

void irq9(){
printk("handler for hardware interrupt \n");
}

void irq10(){
printk("handler for hardware interrupt \n");
}


void irq11(){
printk("handler for hardware interrupt \n");
}

void irq12(){
printk("handler for hardware interrupt \n");
}

void irq13(){
printk("handler for hardware interrupt \n");
}


void irq14(){
printk("handler for hardware interrupt \n");
}

void irq15(){
printk("handler for hardware interrupt \n");
}






















