#include <stdio.h>
#include <stdlib.h>
#define addr 0xffffffff80000000+0xb8000

int main(int argc, char* argv[], char* envp[])
{
    int fd=1;
    //char str[15] = "Sumit Moiz\n\0";
    write(1, "Hello \0", 6);
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
    else {
        while(1) {
            write(fd, "Fucked\n\0", 7);
        }
    }
    /*
    int cnt=0;
    *((char*)addr + cnt++)=49;
    *((char*)addr + cnt++)=0x07;
    while(1) {
        for(int i=0; i<26; i++) {
            cnt = 2;
            *((char*)addr + cnt++)=65+i;
            *((char*)addr + cnt++)=0x07;
        }
    }
    */
    //printf("%d. Hello\n", ++cnt);
    return 0;
}
