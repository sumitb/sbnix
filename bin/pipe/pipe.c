#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int fd[2];
int main(){
        int pid;
	fd[0]=10;
	fd[1]=12;
        printf("%d %d \n",fd[0], fd[1]);
        pipe(fd);
        int status;
        //int *x=(int *)malloc(sizeof(int));
	char buff[100];
	strcpy(buff,"test pipe");
        printf("%d %d \n",fd[0], fd[1]);
        pid = fork();
        if(pid == 0) {
                dup2(1, fd[1]);
                //while(1);
            //    *x=1;
                printf("%s\n",buff);
                //execve("bin/dir_test", NULL, NULL);
		while(1);
        }
        else {
                waitpid(pid, &status, 0);
               /* pid = fork();
                if(pid == 0) {
                        dup2(0, fd[0]);
                        dup2(1,fd[1]);
                        //while(1);
                        execve("bin/scan", NULL, NULL);
                }
                else {*/
                 //       waitpid(pid, &status, 0);
			char rep_buff[100];
                        dup2(0, fd[0]);
                       // dup2(fd[1],1);
                        //execve("bin/cscan", NULL, NULL);
			scanf("%s",rep_buff);
			printf("%s",rep_buff);
                //}
        }
        return 0;
}
