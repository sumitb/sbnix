#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>


int chdir(const char *path)
{
	return syscall_1(SYS_chdir, (uint64_t)path);
}
