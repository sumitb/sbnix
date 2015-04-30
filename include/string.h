#ifndef _STRING_H
#define _STRING_H
#include <sys/defs.h>

int strcpy(char *str1,const char *str2);
int strcmp(const char *str1,const char *str2);
int strcat(char *str1,char *str2);
int strncpy(char *str1,const char *str2, int len);
int strncmp(const char *str1,const char *str2, int len);
int strlen(const char *str);
int itoa(int num, char *buff);
int atoi(char *buff);
int atoo(char *buff);
int otoi(int num);
int xtoa(int num, char *buff);
int atox(char *buff);

void* memset(void *str,int val,uint64_t size);
void memcpy(void *dest, void *src, uint32_t n);
#endif
