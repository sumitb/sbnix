#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int atoi(char *buff)
{
    int i=0;int num=0;
    while(buff[i] != '\0' &&  buff[i] != '\n'){
    num *= 10;
    //num = num + (buff[i]);
            i++;
    }
    printf("%d",num);
    return num;
}

int itoa(int num, char *buff)
{
	//TODO : print in reverse
    int i=0;
    while(num>0){
            buff[i]=num%10 + '0';
            //buff[i]='a';
            num = num/10;
            i++;
    }
    buff[i]='\0';
	write(1,buff,15);
    return 0;
}

int strcpy(char *str1,char *str2){
	int i=0;
	int len = 0;
	len = strlen(str2);
	for(i=0;i<len;i++){
		str1[i]=str2[i];
	}
	str1[i]='\0';
	return 0;
}

int strcmp(char *str1,char *str2){
	int i=0;
	int len1 = 0;int len2 = 0;
	len1 = strlen(str1);
	len2 = strlen(str2);
	if(len1>len2)
		return 1;
	else if(len2>len1)
		return -1;
	else {
		for(i=0;i<len1;i++){
			if(str1[i]>str2[i])
				return 1;
			else if(str2[i]>str1[i])
				return -1;
		}
	}	
	return 0;
}

int strlen(char *str1){
	int len=0;
	while(*str1 != '\0'){
		len++;
		str1++;
	}
	return len;
}


