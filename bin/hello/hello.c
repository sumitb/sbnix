#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLEN 1024

int main(int argc, char* argv[], char* envp[])
{
    char str1[MAXLEN]="\0";
    char str2[MAXLEN]="\0";
    char** env;
    char* thisEnv;
    int i=0;
//  int num2=0;
//  char ch;
//  write(1,buff,sizeof(buff)+1);
struct a{
	int data1;
	char data2[10];	
}*temp;
     
	 int *ptr_one;
       ptr_one = (int *)malloc(sizeof(int));
	  int *ptr_two = (int *)malloc(sizeof(int));
       int *ptr_three = (int *)malloc(sizeof(int));

       if(ptr_one == 0){
            printf("ERROR out of memmory \n");
            return 1;
       }
        *ptr_one=25;
		*ptr_two=30;
		*ptr_three=5;
        printf("%d\n", *ptr_one);
		        printf("%d\n", *ptr_two);
        printf("%d\n", *ptr_three);

        free(ptr_one);
		temp=(struct a *)malloc(sizeof(struct a));
		temp->data1=10;
		strcpy(temp->data2,"abcgdhs");
		printf("%d\n",temp->data1);
		printf("%s\n",temp->data2);
		free(temp);
		     int *ptr_four = (int *)malloc(sizeof(int));
				*ptr_four=4;
				        printf("%d\n", *ptr_four);

        printf("free succes \n");

    printf("Hello World!\n");
    printf("argc = %d\n", argc);
 
  
    for (i=0; i< argc; i++) {
        printf("arg%d=%s\n", i, argv[i]);
    }
    for (env = envp; *env != 0; env++) {
        thisEnv = *env;
        printf("%s\n", thisEnv);
    }

    scanf("%s", str1);
	printf("%s\n", str1);
    scanf("%s", str2);
	printf("%s\n", str2);
	if(strncmp(str1, str2, 3) == 0)
		strncpy(str1, str2, 3);
	else
		strncpy(str1, str2, 4);
		
	printf("%s\n",str1);
	
//  write(1,str,MAXLEN);
/*  scanf("%d",&num);
	printf("abcd \n");
	printf("%d\n",num);
	printf("%d : %s : %d : %s\n",num,str,num,str);
//	itoa(num,buff);
//  write(1,buff,MAXLEN);
    scanf("%x",&num2);
	printf("%x\n",num2);
    scanf("%c",&ch);
	printf("%c\n",ch);
	printf("int :%d string: %s hex: %x char: %c\n",num,str,num2,ch);
*/
    return 0;
}
