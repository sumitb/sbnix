#include <string.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <sys/sched.h>
#include <sys/tarfs.h>
#include <sys/memory.h>
#include <sys/syscall.h>
#include <sys/console.h>

volatile int ggd=0;

int64_t sys_write(uint64_t fildes, char *buf, uint64_t size) {
	struct task_struct *process = getCurrentTask();
	if(process->dup_arr[fildes]!=fildes){
		//write to pipe buffer
		char *pipe_temp_buf=(char *)process->fd[process->dup_arr[fildes]].offset;
		int len=strlen(pipe_temp_buf);
		for(int i=0;i<size;i++,len++)
			pipe_temp_buf[len]=buf[i];
		return size;
	}
	else if(fildes==1 || fildes==2){
		char tem_buf[size];

		strncpy(tem_buf,buf,size);
		tem_buf[size] = '\0';
		return printk(tem_buf);
	}
    /*if(fildes==1 || fildes==2) {
    }
    return 1;
    */
	return 0;
}

int64_t sys_read(uint64_t fildes, char *buf, uint64_t size) {

    if(size>0){
		struct task_struct *process = getCurrentTask();
		if(process->dup_arr[fildes]!=fildes){
			//read from pipe buffer
			char *pipe_temp_buf=(char *)process->fd[process->dup_arr[fildes]].offset;
			strcpy(buf,pipe_temp_buf);
			return strlen(pipe_temp_buf);
		}
		else if(fildes==0){
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
    }
    return 0;
}

pid_t sys_getpid() {
    struct task_struct *proc = getCurrentTask();
    return (proc->pid);
}

pid_t sys_getppid() {
    struct task_struct *proc = getCurrentTask();
    return (proc->ppid);
}

void syscall_handler(){
	uint64_t s_cal_no, param_1, param_2, param_3;
	__asm__ __volatile__("movq %%rax, %0;":"=a"(s_cal_no):);
	__asm__ __volatile__("movq %%rbx, %0;":"=a"(param_1):);
	__asm__ __volatile__("movq %%rsi, %0;":"=a"(param_2):);
	__asm__ __volatile__("movq %%rdx, %0;":"=a"(param_3):);
	
	switch(s_cal_no){
		case SYS_read: //sys_read
            {
                int num=0;
                __asm__ __volatile__ ("sti");
                num = sys_read((uint64_t)param_1, (char*)param_2, (uint64_t)param_3);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)num):"cc", "memory");
            }
			break;
		case SYS_write: //sys_write
            sys_write((uint64_t)param_1, (char*)param_2, (uint64_t)param_3);
            break;
		case SYS_fork:
            {
                pid_t pid = sys_fork();
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)pid):"cc", "memory");
            }
            break;
		case SYS_execve:
            {
                int ret = sys_execve((char*)param_1, (char* const*)param_2, (char* const*)param_3);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)ret):"cc", "memory");
            }
            break;
		case SYS_exit:
            sys_exit(param_1);
			break;
		case SYS_open:
            {
                uint16_t fd = 0;
                fd = sys_open((const char *)param_1, (int)param_2);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)fd):"cc", "memory");
            }
            break;
		case SYS_brk:
            {
                uint64_t addr;
                while(ggd);
                addr = sys_brk(param_1);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)addr):"cc", "memory");
            }
            break;
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
            }
            break;
		case SYS_getdents:
            {
                    uint64_t num_bytes=sys_getdents((int)param_1,(dirent*)param_2,(int)param_3);
                    __asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)num_bytes):"cc", "memory");
        	}
            break;
		case SYS_pipe:
            {
                    int num=sys_pipe((int *)param_1);
                    __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)num):"cc", "memory");
        	}
            break;
		case SYS_dup2:
            {
                    int ret=sys_dup2((int)param_1,(int)param_2);
                    __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)ret):"cc", "memory");
        	}
            break;
	}
}

