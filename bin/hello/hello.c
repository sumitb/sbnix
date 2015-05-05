#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/console.h>
#define MAXLEN 1024
#include<syscall.h>
int listdir(const char *path) {
    struct dirent *entry;
    DIR *dp;

    dp = opendir(path);
      if (dp == NULL) {
        printf("opendir error");
        return -1;
    }
    entry = readdir(dp);
    while((entry = readdir(dp)))
        printf("%s\n", entry->d_name);

    closedir(dp);
    return 0;
}

int test_lseek(const char * path){
    char buf[40];
    uint64_t size;
    int file_des=open(path,0);
       size= read(file_des,buf,30);
       printf("read\n");
      printf("%s",(char*)buf);
       printf("%d",size);
    off_t offset=lseek(file_des,20,0);
    printf("lseek\n");
    printf("%d",offset);
    size=read(file_des,buf,30);
    printf("read\n");
    printf(buf);
    return 0;
}

int main(int argc, char* argv[], char* envp[])
{

   // int num_bytes = syscall_3(SYS_getdents, (uint64_t)9090, (uint64_t)102121, (uint64_t)1025);
  //  printf("%d", num_bytes);
    int s=test_lseek("bin/sbush");
        if(s==0)
            printf("success\n");
        else
            printf("fail\n");
//  char str1[MAXLEN]="\0";
//  char str2[MAXLEN]="\0";
//  char** env;
//  char* thisEnv;
//  int i=0;
//  int num2=0;
//  char ch;
//  write(1,buff,sizeof(buff)+1);
/*struct a{
	int data1;
	char data2[10];	
}*temp;
     
	 int *ptr_one;
       ptr_one = (int *)malloc(sizeof(int));
	  int *ptr_two = (int *)malloc(1024);
       int *ptr_three = (int *)malloc(50);

       if(ptr_one == 0){
            printf("ERROR out of memmory \n");
            return 1;
       }
        *ptr_one=25;
		*ptr_two=30;
		*ptr_three=5;
        memset(ptr_two,0,1024);
        printf("%d\n", *ptr_one);
		printf("%x\n", ptr_two);
		printf("%x\n", ptr_three);
        printf("SS: %d\n", *ptr_three);

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

        printf("free succes \n");*/

    //printf("Hello World!\n");
    //printf("argc = %d\n", argc);
 
  
    /*for (i=0; i< argc; i++) {
        printf("arg%d=%s\n", i, argv[i]);
    }
    for (env = envp; *env != 0; env++) {
        thisEnv = *env;
        printf("%s\n", thisEnv);
    }*/

    //scanf("%s", str1);
	//printf("%s\n", str1);
    //scanf("%s", str2);
	//printf("%s\n", str2);
	/*if(strncmp(str1, str2, 3) == 0)
		strncpy(str1, str2, 3);
	else
		strncpy(str1, str2, 4);
		
	printf("%s\n",str1);*/
	
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
//    listdir("/");
/*	*((char*)0xffffffff80000000+0xb8000)=65;
	*((char*)0xffffffff80000000+0xb8001)=0x07;
    write(1, "ABCD\0", 2);
    pid_t pid = fork();
		if(pid>0)
		{
		        *((char*)0xffffffff80000000+0xb8000)=66;
       			*((char*)0xffffffff80000000+0xb8001)=0x07;
		}
		else if(pid==0)
                {
                        *((char*)0xffffffff80000000+0xb8000)=67;
                        *((char*)0xffffffff80000000+0xb8001)=0x07;
                }
		else 
                {
                        *((char*)0xffffffff80000000+0xb8000)=68;
                        *((char*)0xffffffff80000000+0xb8001)=0x07;
                }
*/
	int fd=0;
	char *str = (char *)malloc(20);
	//int num=0;
	//read(0,str,10);
	printf("Enter string\n");
	scanf("%s",str);
	//if(num>0)
	printf("str :%s\n",str);
	char c;
	int x;
	fd = open("bin/sbush",0);
	printf("1. hello\n");	
	printf("2. Moiz\n");	
	printf("3. Ali\n");	
	printf("file desc :%d\n",fd);
	printf("Enter char\n");
	scanf("%c",&c);
	printf("c : %c\n",c);
	printf("Enter Hex\n");
	scanf("%x",&x);
	printf("x : %x\n",x);
	printf("str : %s,\t c : %c\n",str,c);
	int y=4;
	printf("Enter decimal\n");
	scanf("%d",&y);
	printf("val : %d\n",y);
    while(1);
    printk("Hello World 13 Apr\n");
    return 0;
}
