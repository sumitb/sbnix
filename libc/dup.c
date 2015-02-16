#include <stdio.h>
#include <sys/syscall.h>
#include <syscall.h>

int dup(int oldfd){
	return syscall_1(SYS_dup,(uint64_t)oldfd);
}

