#ifndef _STRING_H
#define _STRING_H

int strcpy(char *str1,char *str2);
int strcmp(char *str1,char *str2);
int strncpy(char *str1,char *str2, int len);
int strncmp(char *str1,char *str2, int len);
int strlen(char *str);
int itoa(int num, char *buff);
int atoi(char *buff);
int xtoa(int num, char *buff);
int atox(char *buff);

#endif
