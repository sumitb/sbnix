#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/console.h>
#define MAXLEN 1024
#include<syscall.h>


   int main(int argc, char* argv[], char* envp[]) {
    struct dirent *entry;
    DIR *dp;
   // char *path=argv[0];
    char *path="bin/";
    printf("test binary for opendir, readdir and closedir system calls\n");
    dp = opendir(path);
      if (dp == NULL) {
              printf("opendir error");
              return -1;
          }
    entry = readdir(dp);
    while((entry = readdir(dp)))
        printf("%s\n", entry->d_name);

    closedir(dp);
    return 0;
}

