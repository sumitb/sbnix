#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>
#define count 1024

void *opendir(const char *name)
{
	int num_bytes;
    //char* buf;
	DIR* dirn = (DIR*)malloc(sizeof(DIR));
	int fd = open(name, O_DIRECTORY | O_RDONLY);

    //buf = malloc(count * sizeof(char));
    //num_bytes = syscall_3(SYS_getdents, (uint64_t)fd, (uint64_t)buf, (uint64_t)count);
    if(fd) {
        num_bytes = syscall_3(SYS_getdents, (uint64_t)fd, (uint64_t)&(dirn->buffer), (uint64_t)count);
        
        dirn->fd = fd;
        dirn->size = num_bytes;
        //dirn->buffer = buf;
        dirn->offset = 0;
        return (void *)dirn;
    }
    return NULL;
 }

