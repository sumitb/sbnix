#include <stdlib.h>
#include <stdio.h>
#define MAXARGS 8
#define PATHVAR 8

int main(int argc, char* argv[], char* envp[]);

void _start(void) {
	int i; 
	int argc = 1;
	char* argv[MAXARGS];
	char* envp[PATHVAR];
	int res;
   // char *envp[]={"/bin:/rootfs/bin","moizali","root"};
    uint64_t* rsp;
    register int64_t esp __asm ("rsp");
    rsp = (uint64_t*)esp;

    // Skip local variables on stack
    //while(* rsp++ == 0);
    while(*rsp++ != 9890){
    	while(* rsp++ == 0);
    	argc =*(--rsp);
    i = 0; while(*rsp != 0) argv[i++] = (char*)(*(++rsp));
    i = 0; rsp++; while(*rsp != 0) envp[i++] = (char*)(*(rsp++));
}	
	printf("reached\n");
    res = main(argc, argv, envp);
	exit(res);
}
