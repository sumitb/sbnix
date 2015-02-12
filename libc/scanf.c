#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAXLENGTH	1024
/*int scani(){
	return 0;
}

int scans(){
	return 0;
}

int scanx(){
	return 0;
}

int scanc(){
	return 0;
}*/

int scanf(const char *format, ...) {
	va_list val;
	int scanned = 0;//int i=0;
	char buf[MAXLENGTH]="\0";
	char *t;int *num;
//	char tmp[15]="\0";
	va_start(val, format);
	int n=0;
	n=read(0, buf, MAXLENGTH);
	buf[n]='\0';
	while(*format) {
		if(*format == '%')
		++format;
		switch(*format){
			case 'd':
				num = va_arg (val, int *);	 	
				*num=atoi(buf);
			//	itoa(*num,tmp);
			//	write(1,tmp,15);
				break;
			case 's':
				t = va_arg (val, char *);
				strcpy(t,buf);
				break;
			case 'x':
				break;
			case 'c':
				t = va_arg (val, char *);
				t[1]='\0';
				break;
		}
		++scanned;
		++format;
	}
	return scanned;
}
