#include <stdio.h>
#include <sys/syscall.h>
#include <syscall.h>

uint64_t read(int fd, void *buf, uint64_t count){
	return syscall_3(SYS_read,(uint64_t)fd, (uint64_t)buf, (uint64_t)count);

}



