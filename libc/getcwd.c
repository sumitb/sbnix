#include <stdlib.h>
#include <sys/syscall.h>
#include <syscall.h>


char *getcwd(char *buf, size_t size)
{
	return (char *) syscall_2(SYS_getcwd, (uint64_t)buf, (uint64_t)size);
} 
