#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[]) {
    char buff[15] = "abcde";
    write(1, buff, sizeof(buff)+1);

//	printf("Hello World!\n");
	return 0;
}
