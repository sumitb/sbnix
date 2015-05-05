#include <stdio.h>
#include <stdlib.h>
#define addr 0xffffffff80000000+0xb8000


int main(int argc, char* argv[], char* envp[])
{
char *str1;
char *str2;
char *str3;
char *str4;
char *str5;
char *str6;
char *str7;
char *str8;
    int fd=1;
    char string = '8';
    //char str[15] = "Sumit Moiz\n\0";
    str1=(char *)malloc(50);
    str2=(char *)malloc(50);
    str3=(char *)malloc(50);
    str4=(char *)malloc(50);
    str5=(char *)malloc(50);
    str6=(char *)malloc(50);
    str7=(char *)malloc(50);
    str8=(char *)malloc(50);
    
    strcpy(str1,"1.hello");
    strcpy(str2,"2.hello");
    strcpy(str3,"3.hello");
    strcpy(str4,"4.hello");
    strcpy(str5,"5.hello");
    strcpy(str6,"6.hello");
    strcpy(str7,"7.hello");
    strcpy(str8,"8.hello");
    write(1, "Hello \0", 6);
    pid_t pid = fork();
    
    write(fd, str1, 8);
    write(fd, str2, 8);
    write(fd, str3, 8);
    write(fd, str4, 8);
    write(fd, str5, 8);
    write(fd, str6, 8);
    write(fd, str7, 8);
    write(fd, str8, 8);
    if(pid > 0) {
        while(1) {
            write(fd, &string, 1);
        }
    }
    else if(pid == 0) {
    char str = '7';
//    	execve("bin/qux", argv, envp);
//        str = '6';
        while(1) {
            write(fd, &str, 1);
        }
    }
    else {
    //	str = '3';
        while(1) {
            write(fd, "9\0", 1);
        }
    }
    write(fd, "Parent\n\0", 7);
    write(fd, "Child\n\0", 6);
    write(fd, "Fuck\n\0", 5);
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
