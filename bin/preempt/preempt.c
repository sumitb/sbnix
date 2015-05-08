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
            for(int i=0;i<1000;i++)
                printf("parent test\n");
            sleep(3);
            printf("Hello Parent\n");
        }


    




}
