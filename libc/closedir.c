#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>


int closedir(void *dir)
{
    DIR* dp;
    dp = (DIR*) dir;
    return close(dp->fd);
}

