#include <stdlib.h>
#include <stdio.h>
#include <syscall.h>
#include <sys/syscall.h>

struct dirent *readdir(void *dir)
{
    DIR* dp;
    dp = (DIR*) dir;
    struct dirent* dirn;

    if(dp->offset >= dp->size)
        return NULL;
    dirn = (struct dirent *)((char *)dp->buffer + dp->offset);
    dp->offset += dirn->d_reclen;
    return dirn;
}

