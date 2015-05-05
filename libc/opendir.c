#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>
#define count 1024
void *opendir(const char *name)
{
	int num_bytes;
    char* buf;
	DIR* dirn = (DIR*)malloc(sizeof(DIR));
    int fd = open(name, O_DIRECTORY | O_RDONLY);
        
    buf = malloc(count * sizeof(char));
    num_bytes = syscall_3(SYS_getdents, (uint64_t)fd, (uint64_t)buf, (uint64_t)count);
   // num_bytes = syscall_3(SYS_getdents, (uint64_t)fd, (uint64_t)102121, (uint64_t)count);
   write(1,"opendir after getdents",50);

    dirn->fd = fd;
   write(1,"opendir after getdents",50);

    dirn->size = num_bytes;
    dirn->buffer = buf;
    dirn->offset = 0;
   write(1,"opendir after getdents",50);
    
    return (void *)dirn;
}

