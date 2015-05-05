#include <stdio.h>
#include <stdlib.h>
#define addr 0xffffffff80000000+0xb8000

int main(int argc, char* argv[], char* envp[])
{
    int error=1, status;
    pid_t childPID;
    printf("%d. Parent\n", error);
    
    if((childPID = fork()) == 0) {
    	//error = execve("bin/qux", argv, envp);
        printf("%d. Infinite child\n", error);
    }
    else {
        waitpid(childPID, &status, 0);
    }
    printf("%d. Parent Infinite\n", error);
    while(1);
    return 0;
}
