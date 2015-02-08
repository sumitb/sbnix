#ifndef _STRING_H
#define _STRING_H

char* strcpy(char* destination, const char* source);
char* strncpy(char* destination, const char* source, size_t num);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, size_t num);
char* strstr(const char* str1, const char* str2);
char* strchr(const char* str, int character);
void* memset(void* ptr, int value, size_t num);
size_t strlen(const char* s);

#endif
