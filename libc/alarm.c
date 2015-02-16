#include <stdlib.h>
#include <sys/syscall.h>
#include <syscall.h>


unsigned int alarm(unsigned int seconds)
{
	return syscall_1(SYS_alarm, (uint64_t)seconds);
} 




