#include <stdlib.h>
#include <stdio.h>
#define MAXARGS 8
#define PATHVAR 8

int main(int argc, char* argv[], char* envp[]);

void _start(void) {
	//int i;
	int argc = 1;
	char** argv;
	char** envp;
	int res;
   // char *envp[]={"/bin:/rootfs/bin","moizali","root"};
    uint64_t* rsp;
    register int64_t esp __asm ("rsp");
    rsp = (uint64_t*)esp+3;

    argc = (int)*rsp;
    argv = (char**)((uint64_t*)rsp + 1);
    envp = (char**)((uint64_t*)argv + argc + 1);
    //printf("%p", rsp);
    // Stack guard, to set argc, argv to default
    /*while(* rsp++ != 9890){
        // Skip local variables on stack
    	while(* rsp++ == 0);
    	argc =*(--rsp);
        i = 0; while(*rsp != 0) argv[i++] = (char*)(*(++rsp));
        i = 0; rsp++; while(*rsp != 0) envp[i++] = (char*)(*(rsp++));
        break;
    }*/
    res = main(argc, argv, envp);
	exit(res);
}
