#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/console.h>


int main(int argc, char* argv[], char* envp[]) {
    int fd;
    char buf[512];
printf("binary file to test open, read and lseek system calls\n");
/*    for(i = 1; i < argc; i++){
            if((fd = open(argv[i])) < 0){
                        printf("cat: cannot open %s\n", argv[i]);
                        exit(0);
                    }
            cat(fd);
            close(fd);
        }*/
    char *filename="bin/baz";
    fd=open(filename,0);
    printf("file descriptor of the opened file %d \n",fd);



    int n;
//    while((n = read(fd, buf, sizeof(buf))) > 0)
        n=read(fd,buf,sizeof(buf));
      printf("%s",buf);
        write(1, buf, n);
        printf("reading file complete\n");
           off_t offset=lseek(fd,20,0);
                printf("lseek\n");
                    printf("%d",offset);
                        n=read(fd,buf,30);
                            printf("read\n");
                                printf(buf);
        if(n < 0){
            printf("cat: read error\n");
            exit(0);
        }


}

