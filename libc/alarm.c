#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>


unsigned int alarm(unsigned int seconds)
{
	return syscall_1(SYS_alarm, (uint64_t)seconds);
}
