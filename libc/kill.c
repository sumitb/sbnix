#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>

void kill(int pid, int sig)
{
	syscall_2(SYS_kill, pid, sig);
}
