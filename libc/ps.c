#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>

void ps()
{
	syscall_0(SYS_ps);
}
