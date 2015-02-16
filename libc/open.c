#include <stdio.h>
#include <sys/syscall.h>
#include <syscall.h>

int open(const char *pathname, int flags){
	return syscall_2(SYS_open,(uint64_t)pathname,(uint64_t)flags);
}



