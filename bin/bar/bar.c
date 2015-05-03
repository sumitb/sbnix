#include <stdio.h>
#include <stdlib.h>
#define addr 0xffffffff80000000+0xb8000

int main(int argc, char* argv[], char* envp[])
{
    int fd=1;
    int arr[100];
    //char str[15] = "Sumit Moiz\n\0";

    for(int i=0; i<100; i++)
        arr[i] = i;
    
    write(fd, &arr, 1);
    write(fd, "World \0", 6);
    pid_t pid = fork();
    
    if(pid > 0) {
        while(1) {
            write(fd, "Parent\n\0", 7);
        }
    }
    else if(pid == 0) {
        while(1) {
            write(fd, "Child\n\0", 6);
        }
    }
    write(fd, "2\0", 1);
    //printf("%d. World\n", ++cnt);
    return 0;
}

