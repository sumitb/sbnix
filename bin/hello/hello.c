#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLEN 1024

int main(int argc, char* argv[], char* envp[]) {
       char buff[15]="\0";
       char str[MAXLEN]="\0";int num=0;
      // write(1,buff,sizeof(buff)+1);

        printf("Hello World!\n");
        scanf("%s",str);
	write(1,str,MAXLEN);
        scanf("%d",&num);
	printf("abcd");
	printf("%d",num);
	itoa(num,buff);
	write(1,buff,MAXLEN);
	
        return 0;
}
