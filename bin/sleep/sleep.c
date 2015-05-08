#include <stdio.h>
#include <stdlib.h>
#define addr 0xffffffff80000000+0xb8000

int main(int argc, char* argv[], char* envp[])
{
    int error=1, status;
    pid_t childPID;
    childPID = fork();
    printf("%d. Parent\n", error);
    sleep(2);
    printf("%d. Parent\n", error);
    sleep(3);
    printf("%d. Parent\n", error);
    sleep(5);

    if(childPID == 0) {
    	//error = execve("bin/qux", argv, envp);
        printf("%d. Child\n", error);
        sleep(2);
        printf("%d. Child\n", error);
        sleep(3);
        printf("%d. Child\n", error);
        sleep(5);
        printf("%d. Infinite child\n", error);
        sleep(11);
    }
    else {
        waitpid(childPID, &status, 0);
    }
    printf("%d. Parent Infinite\n", error);
    while(1);
    return 0;
}

