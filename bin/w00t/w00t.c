#include <stdio.h>
#include <stdlib.h>
#define addr 0xffffffff80000000+0xb8000

int main(int argc, char* argv[], char* envp[])
{
    int error=1, status;
    pid_t childPID;
    
    if((childPID = fork()) == 0) {
    	error = execve("bin/ls", argv, envp);
    }
    else {
        waitpid(childPID, &status, 0);
    }
    printf("%d. Hello\n", error);
    return 0;
}
