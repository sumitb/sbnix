#include <stdlib.h>
#include <sys/syscall.h>
#include <syscall.h>


pid_t waitpid(pid_t pid, int *status, int options)
{
	return (pid_t) syscall_3(SYS_wait4, (uint64_t)pid, (uint64_t)status,(uint64_t)options);
} 



