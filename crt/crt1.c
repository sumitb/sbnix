#include <stdlib.h>
#define MAXARGS 100
#define PATHVAR 100

void _start(void) {
	int i, argc = 1;
	char* argv[MAXARGS];
	char* envp[PATHVAR];
	int res;

    int* rsp;
    char* csp;
    register long esp __asm ("rsp");
    rsp = (int*)esp;

    // Skip local variables on stack
    while(*rsp++ == 0);
    argc = *rsp;
    
    csp = (char*)rsp; csp++;
    i = 0;
    while(*csp++ != 0) argv[i++] = csp; csp++;
    i = 0;
    while(*csp++ != 0) envp[i++] = csp;

    res = main(argc, argv, envp);
	exit(res);
}
