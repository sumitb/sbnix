#include <stdlib.h>
#include <sys/syscall.h>
#include <syscall.h>


pid_t getpid(void)
{
	return syscall_0(SYS_getpid);
} 
