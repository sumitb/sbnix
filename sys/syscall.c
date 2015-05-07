#include <sys/defs.h>
#include <sys/syscall.h>

volatile int ggd=0;

void syscall_handler(){
	uint64_t s_cal_no, param_1, param_2, param_3;
	__asm__ __volatile__("movq %%rax, %0;":"=a"(s_cal_no):);
	__asm__ __volatile__("movq %%rbx, %0;":"=a"(param_1):);
	__asm__ __volatile__("movq %%rsi, %0;":"=a"(param_2):);
	__asm__ __volatile__("movq %%rdx, %0;":"=a"(param_3):);

    /* Ordering maintained from include/sys/syscall.h */
	switch(s_cal_no){
		case SYS_exit:
            sys_exit(param_1);
			break;
		case SYS_brk:
            {
                uint64_t addr;
                while(ggd);
                addr = sys_brk(param_1);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((uint64_t)addr):"cc", "memory");
            }
            break;
		case SYS_fork:
            {
                int64_t pid = sys_fork();
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)pid):"cc", "memory");
            }
            break;
		case SYS_getpid:
            {
                int64_t pid = sys_getpid();
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)pid):"cc", "memory");
                break;
            }
		case SYS_getppid:
            {
                int64_t pid = sys_getppid();
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)pid):"cc", "memory");
            }
            break;
		case SYS_execve:
            {
                int64_t ret = sys_execve((char*)param_1, (char* const*)param_2, (char* const*)param_3);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)ret):"cc", "memory");
            }
            break;
		case SYS_wait4:
            {
                int64_t ret = sys_waitpid((pid_t)param_1, (int*)param_2, (int)param_3);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)ret):"cc", "memory");
            }
            break;
		case SYS_nanosleep:
            {
                int64_t ret = sys_sleep((uint64_t)param_1);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)ret):"cc", "memory");
            }
            break;
		case SYS_getcwd:
            {
                char *dir_name;
                dir_name = sys_getcwd((char *)param_1,(uint64_t)param_2);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)dir_name):"cc", "memory");
                break;
            }
		case SYS_chdir:
            {
                int res;
                res = sys_chdir((char *)param_1);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)res):"cc", "memory");
                break;
            }
		case SYS_open:
            {
                int64_t fd = 0;
                fd = sys_open((const char *)param_1, (int)param_2);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)fd):"cc", "memory");
            }
            break;
		case SYS_read:
            {
                int num=0;
                __asm__ __volatile__ ("sti");
                num = sys_read((uint64_t)param_1, (char*)param_2, (uint64_t)param_3);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)num):"cc", "memory");
            }
			break;
		case SYS_write:
            sys_write((uint64_t)param_1, (char*)param_2, (uint64_t)param_3);
            break;
        case SYS_lseek:
            {
                int64_t offset=sys_lseek((int)param_1,(off_t)param_2,(int)param_3);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)offset):"cc", "memory");
            }
            break;
		case SYS_close:
            {
                int64_t fd = 0;
                fd = sys_close((uint32_t)param_1);
                __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)fd):"cc", "memory");
            }
            break;
		case SYS_getdents:
            {
                    uint64_t num_bytes=sys_getdents((int)param_1, (dirent*)param_2, (int)param_3);
                    __asm__ __volatile__("movq %0, %%rax;" ::"a" ((int64_t)num_bytes):"cc", "memory");
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

