#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
        int pid;
	int fd[2];
        pipe(fd);
        int status;
	char buff[100];
	strcpy(buff,"test pipe");
        pid = fork();
        if(pid == 0) {
                dup2(1, fd[1]);
                execve("bin/ls", NULL, NULL);
        }
        else {
                waitpid(pid, &status, 0);
                dup2(0, fd[0]);
                dup2(fd[1],1);
                execve("bin/scan_pipe", NULL, NULL);
        }
        return 0;
}
