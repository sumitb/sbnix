#include <stdio.h>
#include <sys/syscall.h>
#include <syscall.h>

uint64_t write(int fd, const void *buf, uint64_t count){
	return syscall_3(SYS_write,(uint64_t)fd, (uint64_t)buf, (uint64_t)count);
}
