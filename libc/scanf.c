#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAXLENGTH	1024

int scanf(const char *format, ...) {
	va_list val;
	int scanned = 0;//int i=0;
	char buf[MAXLENGTH]="\0";
	char *t;
	int *num;
	va_start(val, format);
	int n=0;
	n=read(0, buf, MAXLENGTH);
	buf[n-1]='\0';
	while(*format) {
		if(*format == '%')
		++format;
		switch(*format){
			case 'd':
				num = va_arg (val, int *);	 	
				*num=atoi(buf);
				break;
			case 's':
				t = va_arg (val, char *);
				strcpy(t,buf);
				break;
			case 'x':
				num = va_arg (val, int *);	 	
				*num=atox(buf);
				break;
			case 'c':
				t = va_arg (val, char *);
				*t=*buf;
				//write(1,t,1);
				break;
		}
		++scanned;
		++format;
	}
	return scanned;
}
