#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>

pid_t fork(void)
{
    return (pid_t) syscall_0(SYS_fork);
}
