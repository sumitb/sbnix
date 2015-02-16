#include <stdlib.h>
#include <sys/syscall.h>
#include <syscall.h>


int chdir(const char *path)
{
	return syscall_1(SYS_chdir, (uint64_t)path);
} 


