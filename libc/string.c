#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int atoi(char *buff){
        int i=0;int num=0;int neg=1;
	if(buff[i] == '-'){
		neg=-1;
		i++;
	}
        while(buff[i] != '\0' &&  buff[i] != '\n'){
		num = num * 10 + ((buff[i]) - '0');
                i++;
        }
	num = num * neg;
        return num;
}

int atox(char *buff){
        int i=0;int num=0;
        while(buff[i] != '\0' &&  buff[i] != '\n'){
		if(buff[i]>='0' && buff[i] < '9')
			num = num * 16 + ((buff[i]) - '0');
		else
			num = num * 16 + ((buff[i]) - 'a' + 10);
                i++;
        }
        return num;
}

void* memset(void *str,int val,uint64_t size){
    unsigned char* ptr = str;
    while(size--)
        *ptr++ = (unsigned char) val;
    return str;
}

int strrev(char *buff){
	char ch;
	int low=0;int high=0;int len=0;
	len=strlen(buff);
	high = len-1;
	while(high>low){
		ch=buff[low];
		buff[low]=buff[high];
		buff[high]=ch;
		low++;high--;
	}
	return 0;
}

int itoa(int num,char *buff){
        int i=0;
	int neg = 1;
	if(num == 0){
		buff[i]='0';
		i++;
        	buff[i]='\0';
		return 0;
	}
	if(num<0){
		neg = -1;
		num = -num;
	}
	if(num > 0){
        	while(num>0){
               		buff[i]=num%10 + '0';
                	num = num/10;
                	i++;
        	}
	}
	if(neg < 0){
		buff[i]='-';
		i++;
	}
        buff[i]='\0';
	strrev(buff);
        return 0;
}

int xtoa(int num,char *buff){

        int i=0;int rem=0;
	if(num == 0){
		buff[i]='0';
		i++;
        	buff[i]='\0';
		return 0;
	}
	if(num > 0){
        	while(num>0){
			rem = num%16;
			if(rem>9)
				buff[i]=(rem-10) + 'a';
			else
				buff[i]=rem + '0';
                	num = num/16;
                	i++;
        	}
	}
        buff[i]='\0';
	strrev(buff);
        return 0;
}
int strcpy(char *str1,const char *str2){
	int i=0;
	int len = 0;
	len = strlen(str2);
	for(i=0;i<len;i++){
		str1[i]=str2[i];
	}
	str1[i]='\0';
	return 0;
}

int strcat(char *str1,char *str2){
	int i=0;int j=0;
	int len1 = 0,len2=0;
	len1 = strlen(str1);
	len2 = strlen(str2);
	for(i=len1;i<len1+len2;i++){
		str1[i]=str2[j];
		j++;
	}
	str1[i]='\0';
	return 0;
}

int strncpy(char *str1,const char *str2,int cplen){
	int i=0;
	int len = 0;
	len = strlen(str2);
	if(cplen<len)
		len=cplen;
	for(i=0;i<len;i++){
		str1[i]=str2[i];
	}
	str1[i]='\0';
	return 0;
}
int strcmp(const char *str1,const char *str2){
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

int strncmp(const char *str1,const char *str2,int cplen){
	int i=0;
/*	int len1 = 0;int len2 = 0;
	len1 = strlen(str1);
	len2 = strlen(str2);
	if(len1>cplen)
		len1=cplen;
	if(len2>cplen)
		len2=cplen;
	if(len1>len2)
		return 1;
	else if(len2>len1)
		return -1;
	else {*/
		for(i=0;i<cplen;i++){
			if(str1[i]>str2[i])
				return 1;
			else if(str2[i]>str1[i])
				return -1;
		}
//	}
	return 0;
}
int strlen(const char *str1){
	int len=0;
	if(*str1)
	   while(*str1 != '\0'){
		len++;
		str1++;
	   }
	return len;
}


