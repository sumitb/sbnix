#ifndef __SYS_SYSCALL_H
#define __SYS_SYSCALL_H

#include <stdlib.h>

#define SYS_exit       60
#define SYS_brk        12
#define SYS_fork       57
#define SYS_getpid     39
#define SYS_getppid   110
#define SYS_execve     59
#define SYS_wait4      61
#define SYS_nanosleep  35
#define SYS_alarm      37
#define SYS_getcwd     79
#define SYS_chdir      80
#define SYS_open        2
#define SYS_read        0
#define SYS_write       1
#define SYS_lseek       8
#define SYS_close       3
#define SYS_pipe       22
#define SYS_dup        32
#define SYS_dup2       33
#define SYS_getdents   78

int64_t sys_brk(uint64_t bump_addr);

/* Process related Syscalls */
int64_t sys_fork();
int64_t sys_getpid(void);
int64_t sys_getppid(void);
int64_t sys_exit(int error_code);
int64_t sys_kill(int pid, int sig);
int64_t sys_sleep(uint64_t seconds);
int64_t sys_waitpid(pid_t pid, int *status_addr, int options);
int64_t sys_execve(const char *filename, char *argv[], char *envp[]);

int64_t sys_open(const char *file_name, int flags);
char * sys_getcwd();
int64_t sys_close(int32_t filedes);
int64_t sys_getdents(int x,dirent* dirp,int count);
int64_t sys_read(uint64_t fildes, char *buf, uint64_t size);
int64_t sys_write(uint64_t fildes, char *buf, uint64_t size);
int64_t sys_lseek(int fildes, off_t offset, int whence);
int64_t sys_pipe(int fd[]);
int64_t sys_dup2(int old_fd, int new_fd);
#endif
