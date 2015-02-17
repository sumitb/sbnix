#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>


int execve(const char *filename, char *const argv[], char *const envp[])
{
	return syscall_3(SYS_execve, (uint64_t)filename, (uint64_t)argv,(uint64_t)envp);
}
