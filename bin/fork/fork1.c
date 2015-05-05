#include <sys/defs.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[], char* envp[]) {
   uint64_t *test = (uint64_t *)malloc(sizeof(uint64_t));
   *test = 1;
 char *a[]={"-a","-b","-c"};
    char *b[]={"dummy","value","pass"};
   int k = fork();
   if(k == 0)
        execve("bin/bar",a,b);
   else
       printf("\n.");

}
