#include <syscall.h>                                                        
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[]) {
    int k=1;
   // int k=fork();
   //
        if(k==0){
            sleep(4);
            printf("Hello CHild \n");
        }
        else{
            printf("parent sleeping\n");
	    sleep(1);
            printf("Hello Parent\n");
        }
}
