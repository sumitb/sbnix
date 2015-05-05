#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>

struct dirent *readdir(void *dir)
{
    DIR* dp;
    dp = (DIR*) dir;
    struct dirent* dirn;
    write(1,dp->dir_name,256);
    if(dp->offset >= dp->size)
        return NULL;
    dirn = (struct dirent*)(dp->buffer + dp->offset);
    dp->offset += dirn->d_reclen;
    return dirn;
}

