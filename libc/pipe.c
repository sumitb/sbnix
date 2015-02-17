#include <stdio.h>
#include <sys/syscall.h>
#include <syscall.h>

int pipe(int filedes[]){
	return syscall_1(SYS_pipe,(uint64_t)filedes);
}

