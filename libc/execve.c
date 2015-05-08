#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>


int execve(const char *filename, char *argv[], char *envp[])
{
	return syscall_3(SYS_execve, (uint64_t)filename, (uint64_t)argv,(uint64_t)envp);
}
