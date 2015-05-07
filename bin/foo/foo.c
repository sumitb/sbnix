#include <stdio.h>
#include <stdlib.h>
#define addr 0xffffffff80000000+0xb8000


int main(int argc, char* argv[], char* envp[])
{
char *str1;
char *str2;
char *str3;
    int fd=1;
    //char str[15] = "Sumit Moiz\n\0";
    str1=(char *)malloc(50);
    str2=(char *)malloc(50);
    str3=(char *)malloc(50);
    /*str4=(char *)malloc(50);
    str5=(char *)malloc(50);
    str6=(char *)malloc(50);
    str7=(char *)malloc(50);
    str8=(char *)malloc(50);
   */ 
    strcpy(str1,"1.hello");
    strcpy(str2,"2.hello");
    strcpy(str3,"3.hello");
    pid_t pid = fork();
    if(pid > 0) {
    		write(fd, str1, 8);
    		write(fd, str2, 8);
    		write(fd, str3, 8);
		for(int i=0;i<1000;i++){
    		write(fd, str3, 8);
		}
    }
    else if(pid == 0) {
    		char str = '7';
		for(int i=0;i<1000;i++){
            	write(fd, &str, 1);
		}
//    		write(fd, str1, 8);
 //   		write(fd, str2, 8);
  //  		write(fd, str3, 8);
		
    }
    //write(fd, "Parent\n\0", 7);
    //write(fd, "Child\n\0", 6);
    //write(fd, "Fuck\n\0", 5);
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
