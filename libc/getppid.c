#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>


pid_t getppid(void)
{
	return (pid_t) syscall_0(SYS_getppid);
}
