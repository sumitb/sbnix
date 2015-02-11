#include<stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <syscall.h>


void exit(int status){
	syscall_1(SYS_exit, status);
} 
