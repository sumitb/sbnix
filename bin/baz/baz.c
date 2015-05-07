#include <stdio.h>
#include <stdlib.h>
#define addr 0xffffffff80000000+0xb8100

int main(int argc, char* argv[], char* envp[])
{
    /*
    for(int i=0; i<2000; i++)
        write(1, "3\n\0", 2);
    */
    int pid = 0;
    pid=fork();

    if(pid>0){
        while(1) {
            write(1, "THIS IS PARENT 2\n\0", 20);
        }
	}
	else{
        while(1) {
            write(1, "THIS IS CHILD 2\n\0", 20);
        }
	}
    while(1) {
        //write(1, "Demon \0", 6);
    }
    //printf("%d. Demon\n", ++cnt);
    return 0;
}
