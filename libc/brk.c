#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>

int brk(void *end_data_segment)
{
    return syscall_1(SYS_brk, (uint64_t)end_data_segment);
}
