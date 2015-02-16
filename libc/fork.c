#include <stdio.h>
#include <sys/syscall.h>
#include <syscall.h>
#include <sys/defs.h>
pid_t fork(void)
{
	return syscall_0(SYS_fork);

}



