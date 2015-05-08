#ifndef _STDLIB_H
#define _STDLIB_H

#include <sys/defs.h>
#include <string.h>
extern __thread int errno;

void exit(int status);

typedef uint64_t ssize_t;
typedef uint64_t size_t;
typedef uint32_t pid_t;

// memory

void *malloc(size_t size);
void free(void *ptr);
int brk(void *end_data_segment);

// processes

pid_t fork(void);
void ps();
void kill(int pid, int sig);
pid_t getpid(void);
pid_t getppid(void);
int execve(const char *filename, char *const argv[], char *const envp[]);
pid_t waitpid(pid_t pid, int *status, int options);
unsigned int sleep(unsigned int seconds);
unsigned int alarm(unsigned int seconds);

// paths
char *getcwd(char *buf, size_t size);
int chdir(const char *path);

// files
int file_fd;
enum { O_RDONLY = 0, O_WRONLY = 1, O_RDWR = 2, O_CREAT = 0x40, O_DIRECTORY = 0x10000 };
int open(const char *pathname, int flags);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
enum { SEEK_SET = 0, SEEK_CUR = 1, SEEK_END = 2 };
typedef uint64_t off_t;
off_t lseek(int fildes, off_t offset, int whence);
int close(int fd);
int pipe(int filedes[2]);
int dup(int oldfd);
int dup2(int oldfd, int newfd);

// directories
#define NAME_MAX 255
int global_fd;    //  directory
struct dirent {
	long d_ino;
	off_t d_off;                //offset of all the files
	unsigned short d_reclen;
	char d_name [NAME_MAX+1];   //name of all files
};
typedef struct dirent dirent;
struct File{
    char path[32];
    //dirent directory;
    uint64_t offset;
	uint64_t offset_read;
	uint64_t  file_size;
    int flags;
    uint64_t seek;
};
struct opdir {
	int fd;
	long size;
	long offset;
	char buffer[4000];          // REVW
    char dir_name[NAME_MAX+1];
    int status;
    dirent curdirent;
};
typedef struct opdir DIR;
struct dir{
    char dir_name[NAME_MAX+1];
    dirent curdirent;
    int set;
};
typedef struct dir direct;
void *opendir(const char *name);
struct dirent *readdir(void *dir);
int closedir(void *dir);

#endif
