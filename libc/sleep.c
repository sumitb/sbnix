#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>


unsigned int sleep(unsigned int seconds)
{
	return syscall_1(SYS_nanosleep, (uint64_t)seconds);
}
