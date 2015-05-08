#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>
int main() {
    char dir_name[20]; uint64_t size=0;
    printf("%s\n",getcwd(dir_name,size));
    return 0;

}
