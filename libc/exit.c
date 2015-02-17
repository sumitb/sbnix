#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>

void exit(int status)
{
	syscall_1(SYS_exit, status);
}
