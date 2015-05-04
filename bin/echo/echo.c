#include <stdlib.h>
#include <stdio.h>
#include <sys/defs.h>
#include <string.h>


int main(int argc, char* argv[], char* envp[]){
	char buf[30];
	strcpy(buf,"hello");
	for(int i=0; i<5; i++){
		printf("%c", buf[i]);
	}
	while(1);
	return 0;
}
