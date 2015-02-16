#include <stdlib.h>
#include <sys/syscall.h>
#include <syscall.h>


pid_t getppid(void)
{
	return (pid_t) syscall_0(SYS_getppid);
} 
