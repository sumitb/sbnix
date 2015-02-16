#include <stdio.h>
#include <sys/syscall.h>
#include <syscall.h>

int close(int fd){
	return syscall_1(SYS_close,(uint64_t)fd);
}



