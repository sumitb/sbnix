#include <stdlib.h>
#define MAXARGS 10
#define PATHVAR 50

int main(int argc, char* argv[], char* envp[]);

void _start(void) {
	int i, argc = 1;
	char* argv[MAXARGS];
	char* envp[PATHVAR];
	int res;

    uint64_t* rsp;
    register int64_t esp __asm ("rsp");
    rsp = (uint64_t*)esp;

    // Skip local variables on stack
    while(* rsp++ == 0);
    argc = *(--rsp);
    i = 0; while(*rsp != 0) argv[i++] = (char*)(*(++rsp));
    i = 0; rsp++; while(*rsp != 0) envp[i++] = (char*)(*(rsp++));

    res = main(argc, argv, envp);
	exit(res);
}
